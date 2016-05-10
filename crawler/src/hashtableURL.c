/*
* Source code for Hashtable of URLs
*/

#include "HashTableURL.h"


/*************************
* URL Hashtable wrappers 
*************************/

/*
* HashTableAddURL - Wrapper function to add URLs to hashtable
*/
int HashTableAddURL(HashTable* ht, char* url) {
  char* buf = malloc(strlen(url)+1);
  strcpy(buf, url);
  return HashTableAdd(ht, (element_t) buf, HashString);
}

/*
* HashTableLookUpURL - wrapper function to lookup URLs in hashtable
* @ht: Hashtable in which to look
* @url: url looking up
* @buf: pointer to URL if found, NULL otherwise
*/
int HashTableLookUpURL(HashTable* ht, char* url) {
  int found;

  if((found = HashTableLookUp(ht, url, HashString, cmpStrings))) {
    return 1;
  }
  else {
    return 0;
  }
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