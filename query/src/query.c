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
#include "query.h"

static char prompt[] = "Query:> ";


int checkCommandLine(char* filename, char* path);
void eval(char* cmdline);
void ToLower(char* word);
int str_compare(element_t av, element_t bv);
void free_string(element_t av);
void normalizeQuery(char* str);
List* HandleQuery(HashTable* ht, Query* query);
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
  while (1) { 
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
* initQuery - Initializes a search query by parsing the
* search string into a list of search terms and
* a list of search operations
* @str: search string to parse
*
* Returns a pointer to a Query struct
*/
 Query* initQuery(char* str) 
 {
  // Allocate Query struct
  Query* query = calloc(1, sizeof(Query));
  query->terms = calloc(1, sizeof(List));
  query->ops = calloc(1, sizeof(List));
  
  // Initialize the lists
  list_new(query->terms, sizeof(char*), str_compare, free_string);
  list_new(query->ops, sizeof(char*), str_compare, free_string);


  // Normalize search string and parse
  normalizeQuery(str);
  query->num_sets = parseQuery(str, query->terms, query->ops);

  return query;
 }

/*
* normalizeQuery - Normslizes a search query by treating SPACE
* as an AND if missing and returns the number of words in
* the search query. Also converts to all lowercase and removes
* extra characters like '\n'
* @query; string to parse
*
*/
void normalizeQuery(char* query) 
{
  query[strlen(query)-1] = '\0';

}

/*
* parseQuery - Works through str and appends word to
* words and appends logical operators to ops
*
* ASSUMPTIONS: For now assume user specifies all logical operations
* and everything else valid
*/
int parseQuery(char* str, List* terms, List* ops) 
{
  /* TODO - validate string for error conditions */
  char* word;
  char* pch;
  int tokens, sets;

  tokens = 0;
  sets = 1;

  pch = strtok(str, " ");
  while (pch != NULL) {
    word = calloc(1, BUF_SIZE);
    strcpy(word, pch);
    strcat(word, "\0");

    if (strcmp(word, "AND") == 0) { 
      /* Append to ops list */
      list_append(ops, word);
    }
    else if (strcmp(word, "OR") == 0) {
      list_append(ops, word);
      sets++;
    }
    else {
      // Not logical operator
      ToLower(word);
      list_append(terms, word);
    }

    tokens++;
    pch = strtok(NULL, " ");
  }
  return sets;
}

/* 
* ToLower - turns a string into all lower case
* @word: Word to change
*/
void ToLower(char* word) 
{
  char* cur = word;

  while (*cur) {
    tolower(*cur);
    cur++;
  }
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
  while(query->ops->length) {
    op = list_dequeue(query->ops);

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
* getNextQuery - Returns a list of DocumentNode for
* the word at the head of words
* @ht: Hashtable to look in
* @words: list of words nodes to get query from
*/
List* getNextQuery(HashTable* ht, List* words) {
  // TODO - make immutable or return a copy
  char* term;
  /*term = list_dequeue(words);

  WordNode* wNode = calloc(1, sizeof(WordNode));
  hashtable_get(ht, term, wNode);
  free(term);
  return wNode->page;*/
  term = list_dequeue(words);
  WordNode* wNode = calloc(1, sizeof(WordNode)); // Would need to free, but might create dangling pointers
  hashtable_get(ht, term, wNode); //need to check actually return the item
  List* temp_list = wNode->page;  // INVARIANT: wNode->page is immutable
  List* list = calloc(1, sizeof(List));
  list_new(list, sizeof(DocumentNode), dNode_cmp, NULL);
  list_foreach(temp_list, head, next, cur) {
    DocumentNode* data = calloc(1, sizeof(DocumentNode));
    memcpy(data, cur->data, list->elementSize);
    list_append(list, data);
  }
  //free(wNode) //STOP: dangling pointer likely
  free(term);
  return list;
}

/*
* intersect - intersect two lists of DocumentNodes
* Returns pointer to a new list
*/
List* intersect(List* A, List* B) {
  // TODO - probably a lot of mem leaks here,
  // Better way - Use ht
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

int str_compare(element_t av, element_t bv)
{
  char* a = av;
  char* b = bv;
  return strcmp(a, b) == 0;
}

void free_string(element_t av)
{
  char* a = av;
  free(a);
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