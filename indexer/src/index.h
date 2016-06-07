#ifndef _INDEX_H
#define _INDEX_H

#include "../../util/hashtable.h"
#include "../../util/common.h"
#include "../../util/list.h"
#include <pthread.h>

/* Wordnode Hashtable wrappers */
int updateIndex(char* word, intptr_t docID, HashTable* index);
int initWNode(char* word, intptr_t docID, WordNode* wNode);
void dNode_free(element_t elem);
int dNode_cmp(element_t av, element_t bv);
void IndexLoadWords(HashTable* Index, char** str);
void readFile(HashTable* ht, char* filename);
void handleLine(HashTable* index, char* line);
int wNode_cmp(element_t av, element_t bv);
uint32_t wNode_hash(element_t keyv);
void wNode_free(element_t data);

#endif