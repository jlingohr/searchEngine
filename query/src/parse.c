/*
* parse.c - Handle input parsing for the query engine
*/
#include "parse.h"

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
  //ToLower(query);
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
    word = calloc(1, WORD_LENGTH + 1);
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
* getNextQuery - Returns a list of DocumentNode for
* the word at the head of words
* @ht: Hashtable to look in
* @words: list of words nodes to get query from
*/
List* getNextQuery(HashTable* ht, List* words) {
  char* term;

  term = list_dequeue(words);
  WordNode* wNode = calloc(1, sizeof(WordNode)); // Would need to free, but might create dangling pointers
  if (!hashtable_get(ht, term, wNode)) { //need to check actually return the item
    free(wNode);
    free(term);
    return NULL;
  } 
  List* temp_list = wNode->page;  // INVARIANT: wNode->page is immutable
  List* list = calloc(1, sizeof(List));
  list_new(list, sizeof(DocumentNode), dNode_cmp, NULL);
  list_foreach(temp_list, head, next, cur) {
    DocumentNode* data = calloc(1, sizeof(DocumentNode));
    memcpy(data, cur->data, list->elementSize);
    list_append(list, data);
  }
  free(wNode); //dangling pointer in wNode->word?
  free(term);
  return list;
}

/* 
* ToLower - turns a string into all lower case
* @word: Word to change
*/
void ToLower(char* word) 
{
  char* cur = word;

  while (*cur && isalpha(*cur)) {
    tolower(*cur);
    cur++;
  }
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
