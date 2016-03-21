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

#include <stdio.h>
#include <stdlib.h>
#include "crawler.h"
#include "../util/html.h"
#include "../util/hash.h"
#include "../util/header.h"
#include "../util/input.h"
#include <dirent.h>

// Define the dict structure that holds the hash table 
// and the double linked list of DNODES. Each DNODE holds
// a pointer to a URLNODE. This list is used to store
// unique URLs. The search time for this list is O(n).
// To speed that up to O(1) we use the hash table. The
// hash table holds pointers into the list where 
// DNODES with the same key are maintained, assuming
// the hash(key) is not NULL (which implies the URL has
// not been seen before). The hash table provide quick
// access to the point in the list that is relevant
// to the current URL search. 

DICTIONARY* dict = NULL; 


// This is the table that keeps pointers to a list of URL extracted
// from the current HTML page. NULL pointer represents the end of the
// list of URLs.

char *url_list[MAX_URL_PER_PAGE]; 

/*


(5) *Crawler*
-------------

// Input command processing logic

(1) Command line processing on arguments
    Inform the user if arguments are not present
    IF target_directory does not exist OR depth exceeds max_depth THEN
       Inform user of usage and exit failed

// Initialization of any data structures

(2) *initLists* Initialize any data structure and variables

// Bootstrap part of Crawler for first time through with SEED_URL

(3) page = *getPage(seedURL, current_depth, target_directory)* Get HTML into a string and return as page, 
            also save a file (1..N) with correct format (URL, depth, HTML) 
    IF page == NULL THEN
       *log(PANIC: Cannot crawl SEED_URL)* Inform user
       exit failed
(4) URLsLists = *extractURLs(page, SEED_URL)* Extract all URLs from SEED_URL page.
  
(5) *free(page)* Done with the page so release it

(6) *updateListLinkToBeVisited(URLsLists, current_depth + 1)*  For all the URL 
    in the URLsList that do not exist already in the dictionary then add a DNODE/URLNODE 
    pair to the DNODE list. 

(7) *setURLasVisited(SEED_URL)* Mark the current URL visited in the URLNODE.

// Main processing loop of crawler. While there are URL to visit and the depth is not 
// exceeded keep processing the URLs.

(8) WHILE ( URLToBeVisited = *getAddressFromTheLinksToBeVisited(current_depth)* ) DO
        // Get the next URL to be visited from the DNODE list (first one not visited from start)
 
      IF current_depth > max_depth THEN
    
          // For URLs that are over max_depth, we just set them to visited
          // and continue on
    
          setURLasVisited(URLToBeVisited) Mark the current URL visited in the URLNODE.
          continue;

    page = *getPage(URLToBeVisited, current_depth, target_directory)* Get HTML into a 
            string and return as page, also save a file (1..N) with correct format (URL, depth, HTML) 

    IF page == NULL THEN
       *log(PANIC: Cannot crawl URLToBeVisited)* Inform user
       setURLasVisited(URLToBeVisited) Mark the bad URL as visited in the URLNODE.
       Continue; // We don't want the bad URL to stop us processing the remaining URLs.
   
    URLsLists = *extractURLs(page, URLToBeVisited)* Extract all URLs from current page.
  
    *free(page)* Done with the page so release it

    *updateListLinkToBeVisited(URLsLists, current_depth + 1)* For all the URL 
    in the URLsList that do not exist already in the dictionary then add a DNODE/URLNODE 
    pair to the DNODE list. 

    *setURLasVisited(URLToBeVisited)* Mark the current URL visited in the URLNODE.

    // You must include a sleep delay before crawling the next page 
    // See note below for reason.

    *sleep(INTERVAL_PER_FETCH)* Sneak by the server by sleeping. Use the 
     standard Linux system call

(9)  *log(Nothing more to crawl)

(10) *cleanup* Clean up data structures and make sure all files are closed,
      resources deallocated.

*/


int main(int argc, char** argv) {

  int maxDepth;
  int curDepth;
  char* url;
  char* path;
  char* page;
  URLNODE* urlNode;


  
  // Check correct number of arguments passed
  if (argc != 4) {
    perror("Insufficient arguments into crawler.");
    return -1;
  }
  // check valid url passed
  // For now assume valid url passed as specified
  if ((url = isValidSeed(argv[1])) == NULL) {
    perror("Invalid seed.");
    return -1;
  }

  // Check valid target_directory passed
  isValidDir(argv[2]);
  path = argv[2];

  // check correct depth passed
  maxDepth = atoi(argv[3]);
  if (maxDepth > 4 || maxDepth < 0) {
    perror("Depth exceeds maximum allowed.");
    return -1;
  }
  // Initialize variables
  curDepth = 0;

  //initialize data structs
  // create hashtable and call initDictionary to initialize
  dict = malloc(sizeof(DICTIONARY));
  initDictionary(dict);
  // Create list of URLS and call initUrl to initialize
  url_list = malloc(sizeof(char *)*MAX_URL_PER_PAGE);
  urlNode = malloc(sizeof(URLNODE));
  initURLNODE(urlNode, curDepth);

  // Bootstrap from seed
  NormalizeWord(url);
  if (NormalizeUrl(url) == 0) {
    perror("Invalid URL\n");
  }
  addNodeToDictionary(dict, url, node);
  page = getPage(url, curDepth, path);
  if (page == NULL) {
    perror("Invalid html\n");
  }



  return 0;
}

/*
  getPage: Retrives content of a page given url, path, and depth
  @param url - url from which to retrieve contents
  @param depth - current search depth
  @param path - Desintation of file containing page contents
  @return buffer containing contents of the page
**/
char* getPage(char* url, int depth, char* path) {
  NormalizeUrl(url);

  // Create buffer for temp html file before downloading
  char urlBuf[MAX_URL_LENGTH+40];
  // commands to pass to system
  char tempFile[20];
  // form: path/TEMP - used to access temporary html fil
  char pathName[sizeof(path)+15];
  int bufSize;
  int retry;
  int wget_st;
  // used to access page info
  struct stat stat_buf;
  // buffer for html page
  char* file_buffer;
  char file_name[100];

  strcpy(tempFile, path);
  strcat(tempFile, "temp.html");
  bufSize = snprintf(urlBuf, sizeof(urlBuf), "wget -q -T 10 --tries=2 -O %s '%s'", tempFile, url);
  if (bufSize > sizeof(urlBuf)) {
    perror("Command buffer problem");
  }
  retry = 0;
  // Make system call
  if ((wget_st = system(urlBuf)) != 0) {
    free(urlBuf);
    wget_st = 0;
    return NULL;
  }
  strncpy(pathName, path, sizeof(path));
  strncpy(pathName, "TEMP", sizeof("TEMP"));

  stat(pathName, &stat_buf);
  file_buffer = malloc(stat_buf.st_size + 1);
  // Copy temp into a buffer
  initPage(file_buffer, pathName);
  if (file_buffer == NULL) {
    //TODO
  }
  // UNSAFE
  strcpy(file_name, path);
  // Check if file exists

  // If not, open new file and append url/html

  //cleanup



}
