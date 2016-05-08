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
* HashTableAddWNode - Adds a complete WordNode to the table
* @ht: table to add to
* @wNode: WordNode adding
*
*/
void HashTableAddWNode(HashTable* ht, WordNode* wNode) {
  HashTableAdd(ht, wNode, HashWNode);
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
int HashTableLookUpWord(HashTable* ht, char* word) {
  return HashTableLookUp(ht, word, HashString, cmpWNode);
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
  if (!node) {
    fprintf(stderr, "HashTableUpdateWord error\n");
    return 0;
  }

  /* Update tail of WordNode->Page */
  wNode = (WordNode*)node->data;

  dNode = listGetLastDNode(wNode->page);


  if (dNode->document_id == docID) {
    dNode->page_word_frequency++;
  }
  else {
    DocumentNode* tmp = malloc(sizeof(DocumentNode));
    tmp->document_id = docID;
    tmp->page_word_frequency = 1;
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



void concat(element_t* av, element_t bv) {
  char** a = (char**)av;
  char* b = (char*)bv;
  int alen = strlen(*a);
  int blen = strlen(b);
  *a = realloc(*a, alen + blen + 1);
  strcat(*a, b);
}



/*
* HashTableLoadWords - load hashtable values into string buffer. Each word
* is loaded followed by (1) number indicating how many documents the word is in;
* (2) (a,b) pairs where the document a has b occurences of the word
* @ht: Hashtable to load from
* @buf: pointer to C-style string to loads values
*
* Returns size of buffer
*/
int HashTableLoadWords(HashTable* ht, char** buf) {
  /* TODO - REDO, too sloppy and risks fragmentations */
  HashTableNode* node;
  WordNode* wNode;
  char* word, *word_buf;

  for (int i = 0; i < ht->size; i++) {  /* Loop through hashtable */
    node = ht->table[i];
    while (node) { /* Loop through WordNode */
      wNode = (WordNode*)node->data;
      word = wNode->word;
      //printf("Obtaining data on %s\n", word);
      word_buf = malloc(MAXLINE);

      sprintf(word_buf, "%s %d ", word, wNode->page->len);
      listFoldString(concat, &word_buf, wNode->page);
      strcat(*buf, word_buf);
      free(word_buf);

      node = node->next;      
    }
  }
  return strlen(*buf);
}


/*
* cleanHashWord - Cleans up memory for hashtable
* of WordNodes
* @ht: table to clean
*/
void cleanHashWord(HashTable* ht) {
  /* TODO - FInish this */
  HashTableNode* node;
  WordNode* wNode;
  for (int i = 0; i < ht->size; i++) {
    node = ht->table[i];
    while (node) {
      wNode = (WordNode*)node->data;
      listDeleteDNode(wNode->page);
      free(wNode);
      node = node->next;
      /* also free node? */      
    }
    free(ht->table[i]);
  }
  free(ht);
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
  for (int i = 0; i < MAX_HASH_SLOT; i++) {
    ht->table[i] = NULL;
  }
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
int HashTableLookUp(HashTable* ht, element_t key, int (*f)(element_t, int), int (*g)(element_t, element_t)) 
{
  /* TODO - Refactor */
  int p;
  HashTableNode* tmp;

  p = f(key, ht->size);
  tmp = ht->table[p];
  while (tmp) {
    /* Will comparisons always be made on a string? */
    if (g(key, tmp->data) == 0) {
      return 1;
    }
    tmp = tmp->next;
  }
  return 0;
}

