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

#define TRUE 1
static char prompt[] = "Query:> ";


int checkCommandLine(char* filename, char* path);
void eval(char* cmdline);

void handleResults(List* results, char* path);

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

    // Build up query
    query = initQuery(cmdline);

    // Handle the query 
    List* results;  // This is a list of DocumentNodes sorted in order
    results = HandleQuery(Index, query);
    handleResults(results, html_path);
    list_destroy(results);
    list_destroy(query->terms);
    list_destroy(query->ops);
    free(query);
  }
  /* Clean up */
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
* HandleQuery - gets pages matching the query,
* ranks them, and prints them
*/
List* HandleQuery(HashTable* ht, Query* query) 
{

  List* docs;
  int filled, num_sets;

  filled = 0;
  num_sets = query->num_sets;
  List* sets[query->num_sets];

  List* temp_a = getNextQuery(ht, query->terms);
  sets[filled] = temp_a;

  // Store operand, i.e. AND, OR
  char* op;
  while((op = list_dequeue(query->ops))) {
    //op = list_dequeue(query->ops);

    if (strcmp(op, "AND") == 0) {
      // This needs to be imutable or copied
      temp_a = getNextQuery(ht, query->terms);
      sets[filled] = intersect(sets[filled], temp_a);
    }
    else if (strcmp(op, "OR") == 0) {
      filled++;
      sets[filled] = getNextQuery(ht, query->terms);
    }
  }
  //Sort the lists
  List* sorted[num_sets];
  for (int i = 0; i < query->num_sets; i++) {
    sorted[i] = MergeSort(sets[i], sets[i]->length, cmpDNode_freq);
  }
  // Merge all documents 
  docs = calloc(1, sizeof(List));
  list_new(docs, sizeof(DocumentNode), cmpDNode_freq, NULL);
  for (int i = 0; i < query->num_sets; i++) {
    docs = Merge(docs, sorted[i], cmpDNode_freq);
  }
  // Cleanup
  for (int i = 0; i < num_sets; i++) {
    list_destroy(sets[i]);
    list_destroy(sorted[i]);
  }

  return docs;
}

/*
* printResults - Handle results from query by
* opening the relevant document corresponding to each
* DocumentNode in results and fetching the url to
* print to the user
*/
void handleResults(List* results, char* path)
{
  DocumentNode* dNode;
  char filename[MAXLINE];
  int path_len = strlen(path);
  FILE* fd;
  ssize_t read;
  size_t len = 0;
  char* line = NULL;

  strcpy(filename, path);

  ListNode* cur = results->head;
  while (cur) {
    dNode = cur->data;
    sprintf(filename + path_len, "%d", dNode->document_id);

    //open file and retrieve URL
    fd = fopen(filename, "r");
    if ((read = getline(&line, &len, fd)) != -1) {
      if (line) {
        printf("%d  %d %s", dNode->document_id, dNode->page_word_frequency, line);
      }
    }
    fclose(fd);
    cur = cur->next;

  }
}



/*
* intersect - intersect two lists of DocumentNodes
* Returns pointer to a new list
*/
List* intersect(List* A, List* B) {
  List* list;

  if (A == NULL || B == NULL) // empty sets 
    return NULL;

  assert(A != NULL && B != NULL);

  // Initialize new list 
  list = calloc(1, sizeof(List));
  list_new(list, sizeof(DocumentNode), cmpDNode_ID, NULL);

  // Find highest docID of smaller list
  int maxID = 0;
  // Set current pointer to smaller list 
  List* smaller = (list_length(A) < list_length(B)) ? A : B;
  List* larger = (smaller == A) ? B : A;

  ListNode *cur, *_node;
  for (cur = smaller->head, _node = cur; _node != NULL; cur = _node = _node->next) {
    DocumentNode* dNode = (DocumentNode*)cur->data;
    maxID = (dNode->document_id > maxID) ? dNode->document_id : maxID;
  }

  DocumentNode* Intersect[maxID];
  for (int i = 0; i < maxID; i++) {
    Intersect[i] = NULL;
  }
  // Copy DocumentNodes in smaller list into an array
  for (cur = smaller->head, _node = cur; _node != NULL; cur = _node = _node->next) {
    DocumentNode* dNode = (DocumentNode*)cur->data;
    Intersect[dNode->document_id] = dNode;
  }
  // Go through longer list and find matches. If match, copy into a new list
  for (cur = larger->head, _node = cur; _node != NULL; cur = _node = _node->next) {
    DocumentNode* dNode = (DocumentNode*)cur->data;
    if (Intersect[dNode->document_id] != NULL) {
      // Intersect, so copy over
      DocumentNode* in_smaller = Intersect[dNode->document_id];
      DocumentNode* newDNode = calloc(1, sizeof(DocumentNode));
      newDNode->document_id = dNode->document_id;
      newDNode->page_word_frequency = (in_smaller->page_word_frequency < dNode->page_word_frequency) ? in_smaller->page_word_frequency : dNode->page_word_frequency;
      list_append(list, newDNode);
    }
  }
  list_destroy(A);
  list_destroy(B);
  return list;
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
* cmpDNode_freq - Helper function to compare DocumentNode types based
* on the number of word frequencies
*
*/
int cmpDNode_freq(const element_t av, const element_t bv) {
  DocumentNode* a = (DocumentNode*)av;
  DocumentNode* b = (DocumentNode*)bv;

  return (b->page_word_frequency - a->page_word_frequency);
}

/*
* cmpDNode_ID - Helper function to compare DocumentNode types
* based on document ID
*/
int cmpDNode_ID(const element_t av, const element_t bv) {
  DocumentNode* a = (DocumentNode*)av;
  DocumentNode* b = (DocumentNode*)bv;

  return (a->document_id == b->document_id);
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