/*
* Refactored hashtable to work on keys of any kind
*/

#include "hashtable.h"

/*
* initHashTable - Initialize generic hashtable
*/
void initHashTable(HashTable* ht) {
  ht->size = MAX_HASH_SLOT;
  ht->n = 0;
}

/*
* Hash - Generic hash function must be a helper
* based on type of data (i.e. Horners if char*)
*/
unsigned long Hash(const element_t key, unsigned long mod) {
  /* TODO */
  int hash, len;

  hash = 0;
  len = strlen(str);
  for (int i = len-1; i >= 0; i--) {
    hash = (256*hash + str[i]) % mod;
  }
  return hash;
}

/*
* HashTableAdd - Add key to hashtable using chaining
* Creates a new hashtable node and stores key in node
* Returns 1 if successful
*/
int HashTableAdd(int (*f)(element_t), HashTable* ht, element_t key) {
  /* TODO - refactor */
  int p;
  HashTableNode* tmp;


  tmp = malloc(sizeof(HashTableNode));
  tmp->data = key;

  p = f(key);
  if (ht->table[p] == NULL) {
    ht->table[p] = tmp;
    tmp->next = NULL;
  }
  else {
    tmp->next = ht->table[p];
    ht->table[p] = tmp;
  }
  ht->n++;
  return 1;
}

/*
* HashTableLookUp - Searches for key in the table
* Returns 1 if found
* Returns 0 if not found
*/
int HashTableLookUp(int (*f)(element_t), HashTable* ht, void* key) {
  /* TODO - Refactor */
  int p;
  HashTableNode* tmp;

  p = f(key);
  tmp = ht->table[p];
  while (tmp) {
    /* Will comparisons always be made on a string? */
  }


  /*
  tmp = URLSVisited.table[p];
  while (tmp) {
    if (strcmp(url, tmp->url) != 0)
      tmp = tmp->next;
    return 1;
  }*/
  return 0;
}

/*
* cleanHash - Deallocate all memory in hash table
*/
void cleanHash() {
  /* TODO - Refactor */
  HashTableNode* tmp;
  for (int i = 0; i < MAX_HASH_SLOT; i++) {
    tmp = URLSVisited.table[i];
    while (tmp) {
      free(tmp->url)
      tmp = tmp->next;
    }
  }
}