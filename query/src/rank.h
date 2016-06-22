#ifndef _RANK_H
#define _RANK_H

#include <stdio.h>                           // printf
#include <sys/stat.h>                        // isdir
#include <sys/types.h>
#include <unistd.h>
#include <string.h>                          // strncmpr
#include <pthread.h>
#include <assert.h>
#include <ctype.h>

// ---------------- Local includes  e.g., "file.h"
#include "../../util/list.h"                       
#include "../../util/hashtable.h"                       // hashtable functionality
#include "../../util/common.h"                          // common functionality
#include "../../util/util.h"
#include "parse.h"


void handleResults(List* results, char* path);
List* HandleQuery(HashTable* ht, Query* query);
List* intersect(List* A, List* B);

int cmpDNode_ID(const element_t av, const element_t bv);
int cmpDNode_freq(const element_t av, const element_t bv);


#endif