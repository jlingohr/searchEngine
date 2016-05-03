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

// Global:
HashTable URLSVisited;
List toVisit;



/*
(5) *Crawler* Pseudocode
------------------------
    // check command line arguments
    Inform the user if arguments are not present or invalid
    IF target_directory does not exist OR depth exceeds maxDepth THEN
       Inform user of usage and exit failed

    // init curl
     We know from Lab3 that we need to setup curl
    
    // initialize data structures / variables
     Initialize any data structure and variables
    
    // setup seed page

    // get seed webpage
     If it fails, report and exit

    // write seed file

    // add seed page to hashtable

    // extract urls from seed page and add to URLList
  //    making sure not to add them if they're already there
    //    and only if visiting them wouldn't exceed maxDepth

    // while there are urls to crawl do
    
        // get next url from list
 
        // get webpage for url

        // write page file

        // extract urls from webpage and add to URLList
       //    making sure to only add new ones
       //    and only if visiting them wouldn't exceed maxDepth

       // sleep for a bit to avoid annoying the target domain

       // free resources
       
  //  end while    

    // cleanup curl

    // free resources

*/

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
  strcpy(seed_url, argv[1]);
  strcpy(target, argv[2]);
  max_depth = atoi(argv[3]);

  /* Check initLists - Initialize data structures */
  initList();
  depth = 0;
  file_counter = 1;

  /* Bootstrap for initial seed */
  curl_global_init(CURL_GLOBAL_ALL);  // NOTE: Not thread safe!
  seed_page.url = malloc(sizeof(seed_url)+1);
  strcpy(seed_page.url, seed_url);
  strcat(seed_page.url, '/');
  seed_page.html = NULL;
  seed_page.html_len = NULL;
  seed_page.depth = 0;

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

  /* add seed page to hashtable */
  HashTableAdd(seed_page.url, URLSVisited.size);

  /* check updateListLinkToBeVisisted */

  /* Check setURLLast Visisted */

  /*Check getAddressFromTheLinksToBeVisisted */

  /* Check cleanup */

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
    writen(fd, name, strlen(name));
    writen(fd, depth, strlen(depth));
    writen(fd, page->html, page->html_len);
    Fclose(fd);
  }
  return 0;
}
