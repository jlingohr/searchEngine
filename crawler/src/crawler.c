/*

  FILE: crawler.c

  Description:

  Inputs: ./crawler [SEED URL] [TARGET DIRECTORY WHERE TO PUT THE DATA] [MAX CRAWLING DEPTH]

  Outputs: For each webpage crawled the crawler program will create a file in the 
  [TARGET DIRECTORY]. The name of the file will start a 1 for the  [SEED URL] 
  and be incremented for each subsequent HTML webpage crawled. 

  Each file (e.g., 10) will include the URL associated with the saved webpage and the
  depth of search in the file. The URL will be on the first line of the file 
  and the depth on the second line. The HTML will for the webpage 
  will start on the third line.

  To run: ./bin/crawler http://www.cs.dartmouth.edu/~campbell/cs50/ ./data/ 2


*/

// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>                           // printf
#include <sys/stat.h>                        // isdir
#include <sys/types.h>
#include <unistd.h>
#include <curl/curl.h>                       // curl functionality
#include <string.h>                          // strncmpr

// ---------------- Local includes  e.g., "file.h"
#include "utils.h"                           // utility stuffs
//#include "../../util/hashtable.h"                       // hashtable functionality
#include "../../util/common.h"                          // common functionality
#include "../../util/util.h"
#include "../../util/web.h"                             // curl and html functionality
//#include "../../util/list.h"                            // webpage list functionality
#include "hashtableURL.h"
#include "listwpage.h"




// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes
int checkValidInputs(char* seed, char* target, char* max_depth);
int isValidURL(char * URL);
//int isDirec(char * dir);
int writePage(WebPage *page, char *dir, int x);
int crawlPage(WebPage *page);
int saveCrawl();
void cleanup(HashTable* ht);
int validDepth(int depth, int user_depth);

void free_webpage(element_t elem);
int cmp_URL(element_t av, element_t bv);
uint32_t hash_URL(element_t keyv);
void hash_free(element_t data);

// Global:
HashTable* URLSVisited;
List toVisit;


int main(int argc, char** argv) {

  char seed_url[MAX_URL_LENGTH], target[MAX_URL_LENGTH];
  static int user_depth;
  int valid, file_counter;
  WebPage seed_page;

  /* Check command line arguments */
  if (argc != 4) {
    fprintf(stderr, "usage: <url> <directory> <max depth>\n");
    exit(1);
  }
  valid = checkValidInputs(argv[1], argv[2], argv[3]);

  if (!valid) {
    exit(1);
  }
  printf("Crawling - %s\n", argv[1]);

  /* Check initLists - Initialize data structures */
  strcpy(seed_url, argv[1]);
  NormalizeURL(seed_url);
  strcpy(target, argv[2]);
  user_depth = atoi(argv[3]);

  //toVisit= initList();
  list_new(&toVisit, sizeof(WebPage), free_webpage);
  //URLSVisited = initHashTable();
  URLSVisited = malloc(sizeof(HashTable));

  hashtable_new(URLSVisited, MAX_URL_LENGTH, cmp_URL, hash_URL, hash_free);
  file_counter = 1;

  /* Bootstrap given URL for initial seed */
  curl_global_init(CURL_GLOBAL_ALL);  // NOTE: Not thread safe!
  seed_page.url = malloc(sizeof(seed_url));
  strcpy(seed_page.url, seed_url);
  seed_page.html = NULL;
  seed_page.depth = 0;

  if (STATUS_LOG == 1) {
    printf("\nSeed URL: %s", seed_page.url);
  }

  /* Check getPage for seed page */
  if (!GetWebPage(&seed_page)) {
    printf("Coulnd't open seed url: %s\n", seed_page.url);
    free(seed_page.url);
    free(seed_page.html);
    exit(-1);
  }

  /* Write seed page to file */
  if (!writePage(&seed_page, target, file_counter)) {
    printf("Error writing seed page...\n");
    return 1;
  }
  file_counter++;


  /* Extract urls from seed and add to URLLIST
  and Hashtable */
  crawlPage(&seed_page);
  hashtable_insert(URLSVisited, (element_t*)seed_page.url, (element_t*)seed_page.url);
  sleep(INTERVAL_PER_FETCH);

  /* while there are urls to crawl do */
  WebPage* temp_page;
  int result;
  while (list_size(&toVisit)) {
    // Get next URL form the list */
    temp_page = malloc(sizeof(WebPage));
    result = list_dequeue(&toVisit, temp_page);
    if (validDepth(temp_page->depth, user_depth) && !hashtable_lookup(URLSVisited, temp_page->url)) {
      // Get WebPage for URL
      if (GetWebPage(temp_page)) {// write page file
        writePage(temp_page, target, file_counter);
        file_counter++;
        // extract URLs from webpage and add to URLList
        crawlPage(temp_page);
        // Put url into hashtable
        hashtable_insert(URLSVisited, temp_page->url, temp_page->url);

        free(temp_page->url);
        free(temp_page->html); // Free more like URL
      }
    }
    /* free resources */
    free(temp_page);

    /* sleep for a bit to avoid annoying the target domain */
    sleep(INTERVAL_PER_FETCH);
  }

    
    


  /* Cleanup curl */
  curl_global_cleanup();

  /* Free resources */
  cleanup(URLSVisited);
  /* Free seed page */
  free(seed_page.html);
  free(seed_page.url);

  return 0;
}

/*
* checkValidInputs - Parse and validate command line arguments
*/
int checkValidInputs(char* seed, char* target, char* max_depth) {
  int depth;
  

  if (!isValidURL(seed))
    return 1;
  if (!isDirec(target))
    return 1;

  depth = atoi(max_depth);
  if (depth < 0 || depth > MAX_DEPTH) {
    printf("Invalid depth specified: %d\n", depth);
    return 0;
  }  
  
  return 1;

}

/*
* isValidUrl - Checks whether seed is a valid URL and within domain in common.h
* Return 1 if valid
* Return 0 if invalid
*/
int isValidURL(char * URL) {
  if (strncmp(URL, URL_PREFIX, strlen(URL_PREFIX)) != 0)
    return 0;
  return 1;
}

/*
* writePage - Save web page into directory
* URL is saved to first line, depth in second line, and HTML follows
*/
int writePage(WebPage *page, char *dir, int file_counter) {
  char name[MAXLINE];
  char depth[MAXLINE];
  sprintf(name, "%s%d", dir, file_counter);
  sprintf(depth, "\nDepth: %d\n", page->depth);
  FILE* fd = fopen(name, "ab+");
  if (fd) {
    Fputs(page->url, fd);
    Fputs(depth, fd);
    Fputs(page->html, fd);
    fclose(fd);
    return 1;
  }
  if (fd == NULL) {
    fprintf(stderr, "Error opening file.");
    return 1;
  }
  return 0;
}

/*
* crawlPage - Crawls WebPage page to extract URLS
* Extracts urls only if not already in hashtable and would not
* exceed maximum allowed epth
*/
int crawlPage(WebPage *page) {
  int pos;
  char* buf;

  pos = 0;
  buf = NULL;

  while ((pos = GetNextURL(page->html, pos, page->url, &buf)) > 0) {
    if (NormalizeURL(buf)) {
      if (isValidURL(buf)) {
        if (!hashtable_lookup(URLSVisited, buf)) {
          if (STATUS_LOG == 1)
            printf("\nFound url: %s", buf);
          // Fragmentation possible?
          WebPage tmp;
          tmp.url = malloc(strlen(buf)+1);
          strcpy(tmp.url, buf);
          tmp.depth = page->depth;

          //listAddPage(toVisit, tmp);
          list_append(&toVisit, &tmp);
          free(tmp.url);
        }
      }
    }
  }
  free(buf);
  return 1;
}

/*
* validDepth - Checks if depth is less than MAXDEPTH
* Returns 1 if so, else returns 0
*/
int validDepth(int depth, int user_depth) {
  return (depth <= user_depth && depth <= MAX_DEPTH);
}

/*
* cleanup - free resources
*/
void cleanup(HashTable* ht) {
  //cleanHashURL(ht);
  //free(ht);
  hashtable_destroy(ht);
}

/*
* cmp_URL - compares urls
*/
int cmp_URL(element_t av, element_t bv)
{
  // Change this since only need to return 0 or 1
  char* a = av;
  char* b = bv;
  while (*a != 0 && *b != 0 && *a == *b) {
    a++;
    b++;
  }
  return !(*a < *b? -1 : *a == *b? 0 : 1);
}

/*
* hash_URL - Function to hash the url
*/
uint32_t hash_URL(element_t keyv)
{
  char* key = keyv;
  size_t len = strlen(key);

  uint32_t hash = 0;
  uint32_t i = 0;

  for (hash = i = 0; i < len; ++i) {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

/*
* Function to free urls in hashtable
*/
void hash_free(element_t datav)
{
  char* data = datav;
  free(data);
}

/*
* Function to free webpages
*/
void free_webpage(element_t elem)
{
  WebPage* page = elem;
  free(page->url);
  free(page->html);
  free(page);
}