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


static void concat(element_t* outv, element_t strv, element_t dNodev)
{
  char** out = (char**)outv;
  DocumentNode* dNode = dNodev;

  // Copy values into a buffer
  char temp[BUF_SIZE];
  sprintf(temp, "%d %d ", dNode->document_id, dNode->page_word_frequency);
  
  int out_len = strlen(*out);
  int temp_len = strlen(temp);
  *out = realloc(*out, out_len + temp_len + 1);
  strcat(*out, temp);
}

/*
* dNode_concat - Helper to concatenate the data in a list
* of DocumentNode
*/
void dNode_concat(char** str, List* list) 
{
  char* v = malloc(1);
  v[0] = 0;
  list_foldl(concat, (element_t*)&v, list); // iteratively concat DocumentNode values
  strcat(*str, v);
  free(v);
  /*char buf[BUF_SIZE];
  ListNode* node = list->head;
  DocumentNode* dNode;
  while (node) {
    dNode = node->data;
    sprintf(buf, "%d %d ", dNode->document_id, dNode->page_word_frequency);
    node = node->next;
    strcat(*str, buf);

  }
  //strcat(*str, buf);*/

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
      word = wNode->word;
      word_buf = malloc(BUF_SIZE);

      sprintf(word_buf, "%s %d ", word, wNode->page->length); // allocate the word and number of documents
      dNode_concat(&word_buf, wNode->page); // Get string values for each DocumentNode for the current word

      if (strlen(buf) + strlen(word_buf) - 1 <= sizeof(buf)) {
        buf = realloc(buf, 2*sizeof(buf));
      }
      strcat(buf, word_buf); // Concat to the buffer
      strcat(buf, "\n");
      printf("%s\n", word_buf);
      free(word_buf);

      node = node->next;
      //printf("Loaded word: %s\n", word);
    }
  }
  //return buf;
  pthread_exit(&buf);
  
}