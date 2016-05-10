/*
* Default use:
* ./query  [index file] [data file directory]
* 
* Input:

* Output:
*
* To run: 
*
* To Test: 
*/
#include <stdio.h>                           // printf
#include <sys/stat.h>                        // isdir
#include <sys/types.h>
#include <unistd.h>
#include <curl/curl.h>                       // curl functionality
#include <string.h>                          // strncmpr
#include <pthread.h>
#include <assert.h>

// ---------------- Local includes  e.g., "file.h"
#include "../../util/hashtable.h"                       // hashtable functionality
#include "../../util/common.h"                          // common functionality
#include "../../util/util.h"
#include "../../util/web.h"                             // curl and html functionality

static char prompt[] = "Query:> ";


int checkCommandLine(char* filename, char* path);
void eval(char* cmdline);
int handleQuery(char* query, char** urls);
int NormalizeQuery(char* query, char** str);

int main(int argc, char** argv) {
  /* TODO */
  char c;
  char cmdline[MAXLINE];
  char* filename, *html_path;
  int emit_prompt;
  HashTable* index;

  emit_prompt = 1;

  /* Check command line arguments */
  if (argc != 3) {
    fprintf(stderr, "usage: <file.dat> <directory>\n");
    exit(1);
  }
  if (!checkCommandLine(argv[1], argv[2]))
    exit(1);

  char** query;
  int num_words;
  /* What about logical operators? */
  while (1) { 
    if (emit_prompt) {
      printf("%s", prompt);
      fflush(stdout);
    }
    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
      app_error("fgets error");
    if (feof(stdin)) {
      fflush(stdout);
      exit(0);
    }
    filename = argv[1];
    html_path = argv[2];

    /* Build of index from recent file*/
    index = readFile(filename);

    /* Normalize query */
    num_words = NormalizeQuery(cmdline, &query);

    WordNode wNodes[num_words]; /* Static list of WordNodes for each word searched */
    for (int i = 0; i < num_words; i++) {
      wNodes[i] = HashTableGet(index, query[i]);
    }


    /* Parse user query */
    //num_words = HandleQuery(cmdline, &query);


    /* Pass input to ranking module to rank pages */

    /* Print pages in ranked order */

    /* Clean up */
    cleanIndex(index);

  }
  return 0;
}

/*
* checkCommandLine - Checks that filename and path are valid
* @filename: name of file to check
* @path: Directory to check
* 
* Returns 1 if okay
* Returns 0 otherwise
*/
int checkCommandLine(char* filename, char* path) {
  if (!isFile(filename)) {
    printf("%s is not a valid file\n", filename);
    return 0;
  }
  if (!isDir(path)) {
    printf("%s is not a valid directory\n", path);
    return 0;
  }
  return 1;
}

/*
* eval - Parses and handles user query
* @cmdline: query to process
*
*/
void eval(char* cmdline) {
  /* TODO */
  char* urls;
  int num_urls;

  /* Parse command line */
  num_urls = handleQuery(cmdline, &urls);
  Sort(&urls, num_urls);

  /* Print results */
  for (int i = 0; i < num_urls; i++) {
    printf("%s\n", urls[i]);
  }
  printf("\n");

  
}

/*
* handleQuery - parse search query and returns the number
* of urls satisfying request
* @cmdline: string to parse
* @urls: C-style string to store URLS if match
*
* Returns the number of URLs satisying the requst
*/
int handleQuery(char* query, char** urls) {
  /* TODO */
  char* tmp;

  /* Normalize search query string */
  NormalizeQuery(query, &tmp);
}

/*
* NormalizeQuery - Normslizes a search query by treating SPACE
* as an AND if missing and returns the number of words in
* the search query
* @query; string to parse
* @str: C-style string to hold new buffer
*
* Returns number of words in query
*/
int NormalizeQuery(char* query, char** str) {
  /* TODO */
  return -1

}