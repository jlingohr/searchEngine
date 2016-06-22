/*
* parse.c - Handle input parsing for the query engine
*/
#include "parse.h"

/*
* checkQueryString - Validate user search query
* @query: string to validate
* Returns 0 if invalid, else returns 1
*/
int checkQueryString(char* query)
{
  // Check if NULL
  if (!query)
    return 0;
  // Check if empty
  if (strlen(query) == 0)
    return 0;
  int char_count = 0;
  char* cur = query;
  while (*cur && ((*cur == ' ') || (*cur == '\t'))) {
    cur++;
  }
  return *cur != '\n';
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

  // Check that query was valid re not just string of AND/OR
  if (list_length(query->terms) == 0) {
    list_destroy(query->terms);
    list_destroy(query->ops);
    free(query);
    query = NULL;
  }

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
      if (tokens > 0) // 'AND' cant be first term
        list_append(ops, word);
      else
        tokens--;
    }
    else if (strcmp(word, "OR") == 0) {
      if (tokens > 0) { // 'OR' cant be first term
        list_append(ops, word);
        sets++;
      }
      else
        tokens--;
    }
    else {
      if (tokens % 2 == 1) {
        // odd tokens should be logical operatives
        char* tmp = calloc(1, WORD_LENGTH);
        strncpy(tmp, "AND", strlen("AND"));
        strcat(tmp, "\0");
        list_append(ops, tmp);
        tokens++;
      }
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
