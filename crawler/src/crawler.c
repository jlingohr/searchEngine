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

*/

// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>                           // printf
#include <sys/stat.h>                        // isdir
#include <sys/types.h>
#include <unistd.h>
#include <curl/curl.h>                       // curl functionality
#include <string.h>                          // strncmpr

// ---------------- Local includes  e.g., "file.h"
#include "common.h"                          // common functionality
#include "web.h"                             // curl and html functionality
#include "list.h"                            // webpage list functionality
#include "hashtable.h"                       // hashtable functionality
#include "utils.h"                           // utility stuffs
#include "file.h"

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes
int checkValidInputs(char* seed, char* target, char* max_depth);
int isValidURL(char * URL);
int isDirec(char * dir);
int writePage(WebPage *page, char *dir, int x);
int crawlPage(WebPage *page);
int saveCrawl();
void cleanup();
int validDepth(int depth);

// Global:
HashTable URLSVisited;
List toVisit;


int main(int argc, char** argv) {

  char seed_url[MAX_URL_LENGTH], target[MAX_URL_LENGTH];
  int depth, max_depth, valid, file_counter;
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

  strcpy(seed_url, argv[1]);
  NormalizeURL(seed_url);
  printf("Normalized seed URL: %s\n", seed_url);
  strcpy(target, argv[2]);
  max_depth = atoi(argv[3]);

  /* Check initLists - Initialize data structures */
  initList();
  initHashTable();
  depth = 0;
  file_counter = 1;
  printf("Everything initialized...\n");

  /* Bootstrap for initial seed */
  curl_global_init(CURL_GLOBAL_ALL);  // NOTE: Not thread safe!
  printf("Initialized curl...\n");
  seed_page.url = malloc(sizeof(seed_url));
  strcpy(seed_page.url, seed_url);
  //strcat(seed_page.url, "/");
  seed_page.html = NULL;
  //seed_page.html_len = NULL;
  seed_page.depth = 0;
  printf("Bootstrap complete...\n");

  if (STATUS_LOG == 1) {
    printf("\nSeed URL: %s", seed_page.url);
  }

  /* Check getPage */
  if (!GetWebPage(&seed_page)) {
    printf("Coulnd't open seed url: %s\n", seed_page.url);
    free(seed_page.url);
    exit(1);
  }

  /* Write seed file */
  writePage(&seed_page, target, file_counter);
  file_counter++;
  printf("Writing seed page complete...\n");

  /* add seed page to hashtable */
  //HashTableAdd(seed_page.url);    /* Done after crawled! */

  /* Extract urls from seed and add to URLLIST */
  crawlPage(&seed_page);

  /* while there are urls to crawl do */
  WebPage* tmp;
  while(toVisit.head) {
    /* get next url from list */
    tmp = listRemove();

    /* get webpage for url */
    if (GetWebPage(tmp)) { /* write page file */
      writePage(tmp, target, file_counter);
      file_counter++;

      /* extract urls from webpage and add to URLList
       making sure to only add new ones
       and only if visiting them wouldn't exceed maxDepth */
      if (validDepth(tmp->depth))
        crawlPage(tmp); /* Possible asynchronous problems here? */
    }
    /* sleep for a bit to avoid annoying the target domain */
    sleep(INTERVAL_PER_FETCH);

    /* free resources */
    /* TODO - Possible memory leaks doing this! Also free url? */
    free(tmp->html);
    free(tmp);
  }

  /* Cleanup curl */
  curl_global_cleanup();

  /* Free resources */
  cleanup();
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

int isDirec(char* dir) {
  struct stat sbuf;
  Stat(dir, &sbuf);
  if (!S_ISDIR(sbuf.st_mode)) {
    printf("Target %s is not a directory.\n", dir);
    return 0;
  }
  if (!(sbuf.st_mode & S_IWUSR)) {
    printf("%s is not writable.\n", dir);
    return 0;
  }
  return 1;
}

/*
* isValidUrl - Checks whether seed is a valid URL and within domain in common.h
*/
int isValidURL(char * URL) {
  if (strncmp(URL, URL_PREFIX, strlen(URL_PREFIX)))
    return 1;
  return 0;
}

/*
* writePage - Save web page into directory
* URL is saved to first line, depth in second line, and HTML follows
*/
int writePage(WebPage *page, char *dir, int file_counter) {
  // TODO
  char name[MAXLINE];
  char depth[MAXLINE];
  sprintf(name, "%s%d", dir, file_counter);
  sprintf(depth, "\nDepth: %d\n", page->depth);
  FILE* fd = Fopen(name, O_WRONLY);
  if (fd) {
    /*writen(fd, name, strlen(name));
    writen(fd, depth, strlen(depth));
    writen(fd, page->html, page->html_len);
    Fclose(fd);*/
    Fputs(name, fd);
    fprintf(fd, "\nDepth: %s\n", depth);
    Fputs(page->html, fd);
    Fclose(fd);
  }
  return 0;
}

/*
* crawlPage - Crawls WebPage page to extract URLS
*/
int crawlPage(WebPage *page) {
  //TODO - Make sure to sleep!
  int pos;
  char* buf;

  pos = 0;
  buf = NULL;
  while ((pos = GetNextURL(page->html, pos, page->html, &buf)) > 0) {
    if (NormalizeURL(buf)) {
      if (isValidURL(buf)) {
        if (!HashTableLookUp(buf) && page->depth < MAX_DEPTH) { /* Make sure not visited and valid depth */
          if (STATUS_LOG == 1)
            printf("\nFound url: %s", buf);

          WebPage* tmp = malloc(sizeof(WebPage));
          tmp->url = malloc(strlen(buf)+1);
          strcpy(tmp->url, buf);
          tmp->depth = page->depth + 1;

          listAdd(tmp);
          //HashTableAdd(tmp->url);
        }
      }
    }
  }
  HashTableAdd(page->url);
  return 1;
}

/*
* validDepth - Checks if depth is less than MAXDEPTH
* Returns 1 if so, else returns 0
*/
int validDepth(int depth) {
  return depth < MAX_DEPTH;
}

/*
* cleanup - free resources
*/
void cleanup() {
  //TODO
}
