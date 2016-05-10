#ifndef _INDEX_H
#define _INDEX_H

#include "../../util/hashtable.h"
#include "listD.h"

/* Wordnode Hashtable wrappers */
int IndexAddWord(HashTable* ht, char* word, int docID);
int IndexLookUp(HashTable* ht, char* word);
int IndexUpdate(HashTable* ht, char* word, int docID);
int cmpWNode(element_t wordv, element_t wNodev);
int HashWNode(const element_t wNodev, int mod);
WordNode* initWNode(char* word, int docID);
void IndexPrintWords(HashTable* ht);
int IndexLoadWords(HashTable* ht, char** buf);
void cleanIndex(HashTable* ht);

void IndexAddWNode(HashTable* ht, WordNode* wNode);

#endif