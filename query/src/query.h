#ifndef _QUERY_H
#define _QUERY_H

#include <stdio.h>                           // printf
#include <sys/stat.h>                        // isdir
#include <sys/types.h>
#include <unistd.h>
#include <curl/curl.h>                       // curl functionality
#include <string.h>                          // strncmpr
#include <pthread.h>
#include <assert.h>
#include <ctype.h>

// ---------------- Local includes  e.g., "file.h"
#include "../../util/list.h"                       
#include "../../util/hashtable.h"                       // hashtable functionality
#include "../../util/common.h"                          // common functionality
#include "../../util/util.h"
#include "../../util/web.h"                             // curl and html functionality
#include "../../indexer/src/index.h"

typedef struct Query {
  List* terms;          /* The search terms */
  List* ops;            /* Logical operations like AND or OR */
  int num_sets;         /* Number of OR occurences */
} Query;

int NormalizeQuery(char* query);
Query* initQuery(char* str);
int parseQuery(char* str, List* terms, List* ops);
void HandleQuery(HashTable* ht, Query* query);
List* intersect(List* A, List* B);
ListNode* getNextOp(List* list);
List* getNextQuery(HashTable* ht, List* words);

void Sort(List* list, int len);

void update(DocumentNode* a, DocumentNode* b);
int cmpDNode_freq(const element_t av, const element_t bv);
int cmpDNode_ID(const element_t av, const element_t bv);
int Node_strcmp(element_t av, element_t bv);


void printDNode(element_t av);
#endif