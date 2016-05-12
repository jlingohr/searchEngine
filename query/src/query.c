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
Set* handleQuery(HashTable* ht, char** query, char** ops int n);
int NormalizeQuery(char* query, char** str, char** ops);

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
    char* ops;
    num_words = NormalizeQuery(cmdline, &query, &ops);

    /* TODO - how to get queried words? Use union-find?
    */
    //WordNode wNodes[num_words]; /* Static list of WordNodes for each word searched */
    //WordNode* wNodes[num_words];
    //Set* sets[num_words];
    //for (int i = 0; i < num_words; i++) {
      //wNodes[i] = IndexGet(index, query[i]);
     // sets[i] = 
    //}

    Set* matches;
    matches = HandleQuery(index, qury, ops, num_words);

    /* Parse user query */
    //num_words = HandleQuery(cmdline, &query);


    /* Pass input to ranking module to rank pages */
    //char* results;
    rank(wNodes, &results);

    /* Print pages in ranked order */
    display(results);

    /* Clean up */
    free(results);
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
* handleQuery - iteratively create sets to match query
* @ht: Hashtable to find words
* @query: list of query terms
* @n: Number of query search terms (not including logical ops)
*
* Returns a set containing all entries that match the seatch query
*/
Set* handleQuery(HashTable* ht, char** query, char** ops, int n){
  /* TODO */
  Set* s, tmp;

  for (int i = 0; i < n; i++) {
    /* For each query term, make a set from
    the list of DNodes from the hashed value */
    WordNode* wNode = IndexGet(ht, query[i]);
    tmp
  }
}

/*
* NormalizeQuery - Normslizes a search query by treating SPACE
* as an AND if missing and returns the number of words in
* the search query. Also converts to all lowercase
* @query; string to parse
* @str: C-style string to hold new buffer
* @ops: C-style string to store logicl operations parsed
*
* Returns number of words in query
*/
int NormalizeQuery(char* query, char** str, char** ops) {
  /* TODO */
  return -1

}