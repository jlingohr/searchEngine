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
int HashString(const element_t strv, int mod) {
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
  return HashTableAdd(ht, (element_t) buf, HashString);
}

/*
* HashTableLookUpURL - wrapper function to lookup URLs in hashtable
* @ht: Hashtable in which to look
* @url: url looking up
* @buf: pointer to URL if found, NULL otherwise
*/
int HashTableLookUpURL(HashTable* ht, char* url, char** buf) {
  int found;

  if((found = HashTableLookUp(ht, url, (element_t*)buf, HashString, cmpStrings))) {
    *buf = (char*)buf;
  }
  else {
    buf = NULL;
  }
  return found;
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

/*
* HashWNode - Get hash value of a WordNode
* @wNodev: void* pointer to a WordNode
*
* Returns hashed value of WordNode->word
*/
int HashWNode(const element_t wNodev, int mod) {
  WordNode* wNode = (WordNode*)wNodev;
  return HashString(wNode->word, mod);
}

/*
* HashTableAddWord - Add word to hashtable index
* @ht: Hashtable adding to
* @word: Word we are adding to the table
*
* Returns 1 on success
*/
int HashTableAddWord(HashTable* ht, char* word) {
  WordNode* wNode;

  wNode = malloc(sizeof(WordNode));
  strcpy(wNode->word, word);
  return HashTableAdd(ht, wNode, HashWNode);
}

/*
* HashTableLookUpWord - Searches for word in hashtable index
* @ht: Table we are looking in
* @word: Word searching for
* @wNode: Saves pointer to WordNode if held
*
* Returns 1 if word is in table
* Returns 0 otherwise
*/
int HashTableLookUpWord(HashTable* ht, char* word, WordNode** wNode) {
  int found;

  if ((found = HashTableLookUp(ht, word, (element_t*)wNode, HashString, cmpWNode))) {
    *wNode = (WordNode*)wNode;
  }
  else {
    wNode = NULL;
  }
  return found;
}

/*
* cmpWNode - Compares a word and a WordNode
* @wordv: search word
* @wNodev: Node in which to compare
*
* Returns 1 if the same
* Returns 0 otherwise
*/
int cmpWNode(element_t wordv, element_t wNodev) {
  /* TODO - Check casting */
  char* word = (char*)wordv;
  WordNode* wNode = (WordNode*)wNodev;
  return cmpStrings(word, wNode->word);
}

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
* @ht: Hashtable searching in
* @key: Key on which to hash
* @ret: saves pointer to value if found
* @f: hashing function
* @g: comparator function
*
* Returns 1 if found
* Returns 0 if not found
*/
int HashTableLookUp(HashTable* ht, element_t key, element_t* ret,
 int (*f)(element_t, int), int (*g)(element_t, element_t)) 
{
  /* TODO - Refactor */
  int p;
  HashTableNode* tmp;

  p = f(key, ht->size);
  tmp = ht->table[p];
  while (tmp) {
    /* Will comparisons always be made on a string? */
    if (g(key, tmp->data) == 0) {
      *ret = tmp->data;
      return 1;
    }
    tmp = tmp->next;
  }
  ret = NULL;
  return 0;
}

