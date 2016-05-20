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

int main(int argc, char** argv) {
  /* TODO */
  char cmdline[MAXLINE];
  char* filename, *html_path;
  int emit_prompt;
  HashTable* index;
  Query* query;

  emit_prompt = 1;

  /* Check command line arguments */
  if (argc != 3) {
    fprintf(stderr, "usage: <file.dat> <directory>\n");
    exit(1);
  }
  if (!checkCommandLine(argv[1], argv[2]))
    exit(1);

  /* What about logical operators? */
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

    filename = argv[1];
    html_path = argv[2];
    /* Build of index from recent file*/
    index = readFile(filename);

    



    /* Build up query */
    query = initQuery(cmdline);
    printf("Query initialized...\n");
    /* Handle the query */
    HandleQuery(index, query);

    /* Parse user query */
    //num_words = HandleQuery(cmdline, &query);


    /* Pass input to ranking module to rank pages */
    //char* results;
    //rank(wNodes, &results);

    /* Print pages in ranked order */
    //display(results);

    /* Clean up */
    //free(results);
    //cleanIndex(index);

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
* initQuery - Initializes a search query by parsing the
* search string into a list of search terms and
* a list of search operations
* @str: search string to parse
*
* Returns a pointer to a Query struct
*/
 Query* initQuery(char* str) {
  /* Allocate Query struct */
  Query* query = malloc(sizeof(Query));
  query->terms = initList();
  query->ops = initList();

  /* Normalize search string */
  query->num_sets = parseQuery(str, query->terms, query->ops);

  return query;
 }



/*
* parseQuery - Works through str and appends word to
* words and appends logical operators to ops
*
* ASSUMPTIONS: For now assume user specifies all logical operations
* and everything else valid
*/
int parseQuery(char* str, List* terms, List* ops) {
  /* TODO - validate string for error conditions */
  char* word;
  char* pch;
  int tokens, sets;

  tokens = 0;
  sets = 1;

  pch = strtok(str, " ");
  while (pch != NULL) {
    word = malloc(BUF_SIZE);
    strcpy(word, pch);
    strcat(word, "\0");

    if (strcmp(word, "AND") == 0) { 
      /* Append to ops list */
      listAdd(ops, word);
    }
    else if (strcmp(word, "OR") == 0) {
      listAdd(ops, word);
      sets++;
    }
    else {
      /* Not logical operator! */
      ToLower(word);
      listAdd(terms, word);
    }

    tokens++;
    pch = strtok(NULL, " ");
  }
  return sets;
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
int NormalizeQuery(char* query) {
  /* TODO */
  return -1;

}


/* 
* ToLower - turns a string into all lower case
* @word: Word to change
*/
void ToLower(char* word) {
  /* TODO - Check this is corect */
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
void HandleQuery(HashTable* ht, Query* query) {
  /* TODO - Improve this! Too many casts */
  //List *tmp_c; /* List of pointers to lists containing documents */
  List* docs;                           /* Primary list of documents */
  ListNode *opNode;
  int filled, num_sets;

  filled = 0;
  num_sets = query->num_sets;
  List* sets[query->num_sets];

  List* temp_a = getNextQuery(ht, query->terms);
  sets[filled] = temp_a;
  filled++;

  while (filled < num_sets) {
    opNode = listRemove(query->ops);
    char* op = (char*)opNode->data;

    if (strcmp(op, "AND") == 0) {
      temp_a = getNextQuery(ht, query->terms);
      sets[filled] = intersect(sets[filled], temp_a);
    }
    else if(strcmp(op, "OR") == 0) {
      filled++;
      sets[filled] = getNextQuery(ht, query->terms);
    }
  }
  printf("Merging lists...\n");

  /* Sort the list and return */
  for (int i = 0; i < query->num_sets; i++) {
    MergeSort(sets[i], sets[i]->len, cmpDNode_freq);
  }
  /* Merge all documents */
  for (int i = 0; i < query->num_sets; i++) {
    docs = Merge(docs, sets[i], cmpDNode_freq);
  }

  listForEach(printDNode, docs);


}

/*
* getNextQuery - Returns a list of DocumentNode for
* the word at the head of words
* @ht: Hashtable to look in
* @words: list of words nodes to get query from
*/
List* getNextQuery(HashTable* ht, List* words) {
  /* TODO - Make sure this is clean */
  ListNode* tmpNode = listRemove(words);
  char* word = (char*)tmpNode->data;

  WordNode* wNode = IndexGet(ht, word);
  List* acc = wNode->page;
  return acc;
}

/*
* getNextOp - Wrapper to get next operation string
*/
ListNode* getNextOp(List* list) {
  //ListNode* tmp = listRemove(list);
  return NULL;
}

/*
* intersect - intersect two lists
* Returns pointer to a new list
*/
List* intersect(List* A, List* B) {
  /* TODO - NOTE: HUGE mem leaks! */
  List* acc;
  ListNode *cur, *tmp;


  if (A == NULL || B == NULL) /* empty sets */
    return NULL;

  /* Initialize new list */
  acc = initList();

  /* Set current pointer to smaller list */
  if (A->len <= B->len) { /* Have to check if NULL returned */
    while((cur = listRemove(A))) {
      DocumentNode* dNode = (DocumentNode*)cur->data;
      if ((tmp = listGet(B, (element_t)dNode, cmpDNode_ID))) { /* shared item */
        //DocumentNode* dNode = (DocumentNode*)cur->data;
        /* Update document node */
        update(dNode, (DocumentNode*)tmp->data);
        /* Add updated node to accumulated list */
        listAdd(acc, dNode);
      }
    }
  }
  else {
    while((cur = listRemove(A))) {
      DocumentNode* dNode = (DocumentNode*)cur->data;
      if ((tmp = listGet(B, (element_t)dNode, cmpDNode_ID))) { /* shared item */
        //DocumentNode* dNode = (DocumentNode*)cur->data;
        /* Update document node */
        update(dNode, (DocumentNode*)tmp->data);
        /* Add updated node to accumulated list */
        listAdd(acc, dNode);
      }
    }
  } 
  /* Returns list */
  return acc;
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

  return (a->page_word_frequency - b->page_word_frequency);
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