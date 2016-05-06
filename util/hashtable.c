/*
* Refactored hashtable to work on keys of any kind
*/

#include "hashtable.h"

/*************************
* Hashtable helpers
**************************/

/*
* cmpStrings - Helper function to compare strings
*/
int cmpStrings(element_t av, element_t bv) {
  char* a = av;
  char* b = bv;

  while (*a != 0 && *b != 0 && *a == *b) {
    a++;
    b++;
  }
  return *a < *b? -1 : *a == *b? 0 : 1;
}

/*************************
* URL Hashtable wrappers 
*************************/

/*
* Hash - Generic hash function must be a helper
* based on type of data (i.e. Horners if char*)
*/
int HashURL(const element_t strv, int mod) {
  /* TODO */
  int hash, len;
  char* str = strv;

  hash = 0;
  len = strlen(str);
  for (int i = len-1; i >= 0; i--) {
    hash = (256*hash + str[i]) % mod;
  }
  return hash;
}

/*
* HashTableAddURL - Wrapper function to add URLs to hashtable
*/
int HashTableAddURL(HashTable* ht, char* url) {
  char* buf = malloc(strlen(url)+1);
  strcpy(buf, url);
  return HashTableAdd(ht, (element_t) buf, HashURL);
}

/*
* HashTableLookUpURL - wrapper function to lookup URLs in hashtable
*/
int HashTableLookUpURL(HashTable* ht, char* url) {
  return HashTableLookUp(ht, url, HashURL, cmpStrings);
}

/*
* cleanHash - Deallocate all memory in URL hash table
*/
void cleanHashURL(HashTable* ht) {
  HashTableNode *node, *tmp;
  for (int i = 0; i < MAX_HASH_SLOT; i++) {
    node = ht->table[i];
    while (node) {
      free(node->data);
      tmp = node;
      node = node->next;
      free(tmp);
    }
  }
}

/*************************
* Wordnode Hashtable wrappers 
**************************/


/*************************
* Hashtable Macros 
**************************/

/*
* initHashTable - Initialize generic hashtable
*/
HashTable* initHashTable() {
  HashTable* ht = malloc(sizeof(HashTable));
  ht->size = MAX_HASH_SLOT;
  ht->n = 0;
  return ht;
}



/*
* HashTableAdd - Add key to hashtable using chaining
* Creates a new hashtable node and stores key in node
* - ht is pointer to desired hash table
* - key is key to hash
* - f is function pointer to hashing function 
* Returns 1 if successful
*/
int HashTableAdd(HashTable* ht, element_t key, int (*f)(element_t, int)) {
  /* TODO - refactor */
  int p;
  HashTableNode* tmp;


  tmp = malloc(sizeof(HashTableNode));
  tmp->data = key;

  p = f(key, ht->size);
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
* - g is function pointer to comparator function
* Returns 1 if found
* Returns 0 if not found
*/
int HashTableLookUp(HashTable* ht, void* key, int (*f)(element_t, int), int (*g)(element_t, element_t)) {
  /* TODO - Refactor */
  int p;
  HashTableNode* tmp;

  p = f(key, ht->size);
  tmp = ht->table[p];
  while (tmp) {
    /* Will comparisons always be made on a string? */
    if (g(key, tmp->data) == 0)
      return 1;
    tmp = tmp->next;
  }
  return 0;
}

