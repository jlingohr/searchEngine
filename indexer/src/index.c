/*
* index.c - Procedures here manage the relationship between the HashTable of
* WordNodes and the List that each WordNode has
*/

#include "index.h"

/********************
* Private functions
********************/



/*************************
* Wordnode Hashtable wrappers 
**************************/

/* updateIndex - Updates the structure containing the index by
* inserting a new WordNode if not already in the index
* and updating the DocumentNode if word is already
* in the index.

* If word is not in the index, procedure allocates
* a new WordNode and adds to the hashtable
*
* @word: string containing the word
* @docID: identifier of the document
* @index: InvertedIndex hashtable
*
* Returns 1 if successful
* Returns 0 otherwise
*/
int updateIndex(char* word, int docID, HashTable* index) {

  /*if (IndexLookUp(index, word)) { // Word is in table 
    // Update docID for word 
    //return IndexUpdate(index, word, docID);
    //hashtable_update(index, word, docID);
    WordNode* wNode = NULL;
    if (hashtable_get(index, word, wNode)) {

    }
  }
  else {
    // Word not in, so insert word 
    //return IndexAddWord(index, word, docID);
    WordNode* wNode = initWNode(word, docID);
    hashtable_add(index, word, wNode);
  }
  return 1;*/
  WordNode* wNode = NULL;
  if (hashtable_lookup(index, word, wNode)) {
    // word is in hashtable, so find DocumentNode
    DocumentNode* dNode = malloc(sizeof(DocumentNode));
    if (list_get(wNode->page, docID, dNode)) {
      // Update page count
      dNode->page_word_frequency++;
      // remove, update
    } else {
      // Not found, so just add the DocumentNode
      dNode->document_id = docID;
      dNode->page_word_frequency = 1;
    }
    list_append(wNode->page, dNode);
    dNode_free(dNode);
  } else {
    //wNode_free(wNode);
    wNode = initWNode(word, docID);
    hashtable_insert(index, word, wNode);
  }
}

/*
* dNode_free - Function to free DocumentNode
*/
void dNode_free(element_t elem)
{
  DocumentNode* dNode = elem;
  free(elem);
}

/*
* dNode_cmp - Compares a DocumentNodes ID with a docID
* for equality
*/
int dNode_cmp(element_t av, element_t bv)
{
  DocumentNode* a = av;
  intptr_t b = bv;
  return a->document_id == b;
}

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

  //wNode->page = initList();
  list_new(wNode->page, sizeof(DocumentNode), dNode_cmp, dNode_free);


  DocumentNode* dNode = malloc(sizeof(DocumentNode));
  dNode->document_id = docID;
  dNode->page_word_frequency = 1;

  //listAddDoc(wNode->page, dNode);
  list_append(wNode->page, dNode);
  return wNode;
}


/*
* dNode_concat - Helper to concatenate the data in a list
* of DocumentNode
*/
void dNode_concat(element_t* av, element_t bv) {
  char** a = (char**)av;
  DocumentNode* dNode = (DocumentNode*)bv;

  char dNode_buf[MAXLINE];
  sprintf(&dNode_buf, "%d %d ", dNode->document_id, dNode->page_word_frequency);


  int alen = strlen(*a);
  int blen = strlen(dNode_buf);
  *a = realloc(*a, alen + blen + 1);
  strcat(*a, &dNode_buf);
}

/*
* IndexLoadWords - load hashtable values into string buffer. Each word
* is loaded followed by (1) number indicating how many documents the word is in;
* (2) (a,b) pairs where the document a has b occurences of the word
* @ht: Hashtable to load from
* @buf: pointer to C-style string to loads values
*
* Returns size of buffer
* TODO - Hoe to make private?
*/
int IndexLoadWords(HashTable* ht, char** buf) {
  /*
  // TODO - REDO, too sloppy and risks fragmentations 
  HashTableNode* node;
  WordNode* wNode;
  char* word, *word_buf;

  for (int i = 0; i < ht->size; i++) {  // Loop through hashtable 
    node = ht->table[i];
    while (node) { // Loop through WordNode 
      wNode = (WordNode*)node->data;
      word = wNode->word;
      //printf("Obtaining data on %s\n", word);
      word_buf = malloc(MAXLINE);

      sprintf(word_buf, "%s %d ", word, wNode->page->len);
      listFoldString(concat, &word_buf, wNode->page);

      if (strlen(*buf) + strlen(word_buf) - 1 <= sizeof(*buf))
        buf = realloc(buf, 2*sizeof(buf));
      strcat(*buf, word_buf);
      strcat(*buf, "\n");
      free(word_buf);

      node = node->next;      
    }
  }
  return strlen(*buf);*/

  WordNode* wNode;
  char *word, *word_buf;
  for (int i = 0; i < MAX_HASH_SLOT; i++) { // Go through each hashtable bucket
    HashTableNode* node = ht->table[i];
    while (node) {  // Go through each word node
      wNode = node->data;
      word = wNode->word;
      word_buf = malloc(MAXLINE);

      sprintf(word_buf, "%s %d ", word, wNode->page->length);
      list_foldl(dNode_concat, &word_buf, wNode->page);

      if (strlen(*buf) + strlen(word_buf) - 1 <= sizeof(*buf)) {
        buf = realloc(buf, 2*sizeof(buf));
      }
      strcat(*buf, word_buf);
      strcat(*buf, "\n");
      free(word_buf);

      node = node->next;
    }
  }
  return strlen(*buf);
  
}