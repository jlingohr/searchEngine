#include "hashtable.h"

/*
* Hash - Hash url using Horners rule
*/
unsigned long Hash(const char *str, unsigned long mod) {
  //TODO
  int hash, len;
  hash = 0;
  len = strlen(str);
  for (int i = len-1; i >= 0; i--) {
    h = (256*hash + s[i]) % mod;
  }
  return hash;
}

/*
* HashTableAdd - Add url to hashtable using chaining
*/
int HashTableAdd(char * url) {
  HashTableNode* tmp = malloc(sizeof(HashTableNode));
  tmp->url = malloc(strlen(url));
  strcat(tmp->url, url);

  p = Hash(url);
  if (URLSVisited.table[p] == NULL) {
    tmp->next = NULL;
    URLSVisited.table[p] = tmp;
  }
  else {
    tmp->next = URLSVisited.table[p];
    URLSVisited.table[p] = tmp;
  }
  return 1;
}

int HashTableLookUp(char * url) {
  //TODO
}