#ifndef _HASHTABLEURL_H
#define _HASHTABLEURL_H

#include "../../util/hashtable.h"

/* URL Hashtable wrappers */
int HashTableAddURL(HashTable* ht, char* url);
int HashTableLookUpURL(HashTable* ht, char* url);
void cleanHashURL(HashTable* ht);


#endif