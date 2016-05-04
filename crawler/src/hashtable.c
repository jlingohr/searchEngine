#include "hashtable.h"

/*
* initHashTable - Initialize hashtbale
*/
void initHashTable() {
  URLSVisited.size = MAX_HASH_SLOT;
  URLSVisited.n = 0;
}

/*
* Hash - Hash url using Horners rule
*/
unsigned long Hash(const char *str, unsigned long mod) {
  int hash, len;

  hash = 0;
  len = strlen(str);
  for (int i = len-1; i >= 0; i--) {
    hash = (256*hash + str[i]) % mod;
  }
  return hash;
}

/*
* HashTableAdd - Add url to hashtable using chaining
* Returns 1 if successful
*/
int HashTableAdd(char * url) {
  int p;
  HashTableNode* tmp;


  tmp = malloc(sizeof(HashTableNode));
  tmp->url = malloc(strlen(url));
  strcat(tmp->url, url);

  p = Hash(url, URLSVisited.size);
  if (URLSVisited.table[p] == NULL) {
    tmp->next = NULL;
    URLSVisited.table[p] = tmp;
  }
  else {
    tmp->next = URLSVisited.table[p];
    URLSVisited.table[p] = tmp;
  }
  URLSVisited.n++;
  return 1;
}

/*
* HashTableLookUp - Searches for URL in the table
* Returns 1 if found
* Returns 0 if not found
*/
int HashTableLookUp(char * url) {
  int p;
  HashTableNode* tmp;

  p = Hash(url, URLSVisited.size);
  if (URLSVisited.table[p] == NULL)
    return 0;

  tmp = URLSVisited.table[p];
  while (tmp) {
    if (strcmp(url, tmp->url))
      return 1;
    tmp = tmp->next;
  }

  return 0;

}