#ifndef _INDEX_H
#define _INDEX_H

#include "../../util/hashtable.h"
#include "../../util/common.h"
#include "../../util/list.h"

/* Wordnode Hashtable wrappers */
int updateIndex(char* word, int docID, HashTable* index);
WordNode* initWNode(char* word, int docID, WordNode* wNode);
void dNode_free(element_t elem);
int dNode_cmp(element_t av, element_t bv);
int IndexLoadWords(HashTable* ht, char** buf);
void dNode_concat(char** str, List* list);

#endif