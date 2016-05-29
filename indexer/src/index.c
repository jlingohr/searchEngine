/*
* index.c - Procedures here manage the relationship between the HashTable of
* WordNodes and the List that each WordNode has
*/

#include "index.h"

#define eos(s) ((s) + strlen(s))


/********************
* Private functions
********************/



/*************************
* Wordnode Hashtable wrappers 
**************************/


/*
* initWNode - Initializes WordNode and assigns initial values
* @word: Word the node corresponds to
* @docID: documet id
*
*/
WordNode* initWNode(char* word, int docID, WordNode* wNode) {
  /* TODO - BUGS here with assigning dNOde to list;
  Check for memory leaks */
  
  //WordNode* wNode = malloc(sizeof(WordNode));
  strcpy(wNode->word, word);

  //wNode->page = initList();
  wNode->page = malloc(sizeof(List));
  list_new(wNode->page, sizeof(DocumentNode), dNode_cmp, dNode_free);


  DocumentNode* dNode = malloc(sizeof(DocumentNode));
  dNode->document_id = docID;
  dNode->page_word_frequency = 1;

  //listAddDoc(wNode->page, dNode);
  list_append(wNode->page, dNode);
  return wNode;
}

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
int updateIndex(char* word, int docID, HashTable* index) 
{
  //WordNode* wNode = initWNode(word, docID);
  WordNode* wNode = malloc(sizeof(WordNode));
  if (hashtable_get(index, word, wNode)) { //hashtable_lookup(index, word)
    // word is in hashtable, so find DocumentNode
    assert(!strcmp(word, wNode->word));
    DocumentNode* dNode = malloc(sizeof(DocumentNode));
    dNode->document_id = docID;
    if (list_get(wNode->page, &docID, dNode)) {
      // Update page count
      dNode->page_word_frequency++;
      // remove, update
    } else {
      // Not found, so just add the DocumentNode
      //dNode->document_id = docID;
      dNode->page_word_frequency = 1;
    }
    list_append(wNode->page, dNode);
    dNode_free(dNode);
  } else {
    //wNode_free(wNode);
    initWNode(word, docID, wNode);

    hashtable_insert(index, word, wNode);
  }
  return 1;
}

/*
* dNode_free - Function to free DocumentNode
*/
void dNode_free(element_t elem)
{
  DocumentNode* dNode = elem;
  free(dNode);
}

/*
* dNode_cmp - Compares a DocumentNodes ID with a docID
* for equality
*/
int dNode_cmp(element_t av, element_t bv)
{
  DocumentNode* b = bv;
  int* a = av;
  return b->document_id == *a;
}



static void wNode_concat(WordNode* wNode, char** str)
{
  //char headers[MAXLINE];
  char* docs = malloc(BUF_SIZE);
  sprintf(docs, "%s %d ", wNode->word, wNode->page->length);

  ListNode* node = wNode->page->head;
  DocumentNode* dNode;
  //char temp[MAXLINE];
  while (node) {
    DocumentNode* dNode = node->data;
    sprintf(eos(docs), "%d %d ", dNode->document_id, dNode->page_word_frequency);
    //strcat(docs, temp);
    node = node->next;
  }
  strcat(docs, "\n");
  if (strlen(docs) + strlen(*str) - 1 >= sizeof(*str)) {
    *str = realloc(*str, 2*(sizeof(*str)));
  }
  printf("%s", docs);
  strcat(*str, docs);

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
element_t IndexLoadWords(element_t Indexv) 
{ // TODO - this is bugging out
  HashTable* ht = Indexv;
  char* buf = malloc(BUF_SIZE);
  WordNode* wNode;
  char *word, *word_buf;
  for (int i = 0; i < MAX_HASH_SLOT; i++) { // Go through each hashtable bucket
    HashTableNode* node = ht->table[i];
    while (node) {  // Go through each word node

      wNode = node->data;
      wNode_concat(wNode, &buf);

      node = node->next;
      
    }
  }

  return buf;
  
}