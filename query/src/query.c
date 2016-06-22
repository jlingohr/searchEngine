/*
* Default use:
* ./query  [index file] [data file directory]
* i.e.      ./bin/query ../indexer/data/index.dat ../crawler/data/
* 
* Input:

* Output:
*
* To run: 
*
* To Test: 
*/
#include "parse.h"
#include "rank.h"

#define TRUE 1
static char prompt[] = "Query:> ";


int checkCommandLine(char* filename, char* path);
void eval(char* cmdline);


void sigint_handler(int sig);


int main(int argc, char** argv) 
{ 
  char cmdline[MAXLINE];
  char* filename, *html_path;
  int emit_prompt;
  HashTable* Index;
  Query* query;

  emit_prompt = 1;

  filename = argv[1];
  html_path = argv[2];

  // Check command line arguments 
  if (argc != 3) {
    fprintf(stderr, "usage: <file.dat> <directory>\n");
    exit(1);
  }
  if (!checkCommandLine(argv[1], argv[2]))
    exit(1);

  // Register handler
  Signal(SIGINT, sigint_handler);

  // Build of index from recent file
  Index = calloc(1, sizeof(HashTable));
  hashtable_new(Index, sizeof(WordNode), wNode_cmp, wNode_hash, wNode_free);
  readFile(Index, filename);
  // What about logical operators? 
  while (TRUE) { 
    if (emit_prompt) {
      printf("%s", prompt);
      fflush(stdout);
    }
    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
      fprintf(stderr, "fgets error");
    if (feof(stdin)) {
      fflush(stdout);
      exit(0);
    }
    if (!checkQueryString(cmdline)) {
      //free(cmdline);
      continue;
    }
    // Build up query
    if ((query = initQuery(cmdline))) {
      // Handle the query 
      List* results;  // This is a list of DocumentNodes sorted in order
      results = HandleQuery(Index, query);
      handleResults(results, html_path);
      list_destroy(results);
      list_destroy(query->terms);
      list_destroy(query->ops);
      free(query);
    }
  }
  // Clean up 
  hashtable_destroy(Index);
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
int checkCommandLine(char* filename, char* path) 
{
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
* update - Update DocumentNode a with word freq of
* DocumentNode b. 
* NOTE: For now taking the min (from intersection)
*/
void update(DocumentNode* a, DocumentNode* b) {
  if (a->page_word_frequency > b->page_word_frequency)
    a->page_word_frequency = b->page_word_frequency;
}


/*
* Node_strcmp - Helper to compare a node with a string
*/
int Node_strcmp(element_t av, element_t bv) {
  char* a = (char*)av;
  char* b = (char*)bv;
  return strcmp(a, b);
}

void printDNode(const element_t av) {
  DocumentNode* a = (DocumentNode*)av;
  printf("%d, %d\n", a->document_id, a->page_word_frequency);

}


/*
* sigquit_handler - Terminate program whenever user types ctrl-c
*/
void sigint_handler(int sig)
{
  if (sig == SIGINT) {
    printf("\nExit with Ctrl-d\n");
  }
}