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
  char* tmp;

  if((found = HashTableLookUp(ht, url, (element_t*)&tmp, HashString, cmpStrings))) {
    *buf = tmp;
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
* initWNode - Initializes WordNode and assigns initial values
* @word: Word the node corresponds to
* @docID: documet id
*
*/
WordNode* initWNode(char* word, int docID) {
  /* TODO - BUGS here with assigning dNOde to list;
  Check for memory leaks */
  
  WordNode* wNode = malloc(sizeof(WordNode));
  strcpy(wNode->word, word);

  wNode->page = initList();

  DocumentNode* dNode = malloc(sizeof(DocumentNode));
  dNode->document_id = docID;
  dNode->page_word_frequency = 1;

  listAddDoc(wNode->page, dNode);
  return wNode;
}


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
* @docID: document id for word
*
* Returns 1 on success
*/
int HashTableAddWord(HashTable* ht, char* word, int docID) {
  WordNode* wNode;

  wNode = initWNode(word, docID);
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
  WordNode* tmp;

  if ((found = HashTableLookUp(ht, word, (element_t*)&tmp, HashString, cmpWNode))) {
    *wNode = tmp;
  }
  else {
    wNode = NULL;
  }
  return found;
}


/*
* HashTableUpdateWord - Updates word frequency count on relevant doc
* @ht: Hashtable to look in
* @word: Word to update
* @docID: Document id to update
*
* Returns 1 on success 
*/
int HashTableUpdateWord(HashTable* ht, char* word, int docID) {
  HashTableNode* node;
  WordNode* wNode;
  DocumentNode* dNode;
  int p;

  /* Get WordNode for word */
  p = HashString(word, ht->size);
  node = ht->table[p];
  while (cmpWNode(word, node->data) != 0) {
    node = node->next;
  }

  /* Update tail of WordNode->Page */
  wNode = (WordNode*)node->data;
  dNode = (DocumentNode*)wNode->page->tail;

  if (dNode->document_id == docID)
    dNode->page_word_frequency++;
  else {
    DocumentNode* tmp = malloc(sizeof(DocumentNode));
    tmp->document_id = docID;
    tmp->page_word_frequency++;
    listAddDoc(wNode->page, tmp);
  }
  return 1;
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

/*
* HashTablePrintWords - Print out contents of the hashtable
* for testing purposes
* @ht: Hashtable to print out
*/
void HashTablePrintWords(HashTable* ht) {
  /* TODO */
  return
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

