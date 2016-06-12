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
int initWNode(char* word, int docID, WordNode* wNode) {
  /* TODO - BUGS here with assigning dNOde to list;
  Check for memory leaks */
  wNode->word = calloc(1, strlen(word)+1);
  strcpy(wNode->word, word);

  wNode->page = calloc(1, sizeof(List));
  list_new(wNode->page, sizeof(DocumentNode), dNode_cmp, NULL);


  DocumentNode* dNode = calloc(1, sizeof(DocumentNode));
  dNode->document_id = docID;
  dNode->page_word_frequency = 1;

  list_append(wNode->page, dNode);
  return 1;
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
  WordNode* wNode = calloc(1, sizeof(WordNode));
  if (!hashtable_get(index, word, wNode)) {
    assert(!hashtable_lookup(index, word));
    initWNode(word, docID, wNode);
    hashtable_insert(index, word, wNode);
  } else {
    // Word in table, so update its list of DocumentNode
    assert(hashtable_lookup(index, word));
    DocumentNode* dNode = NULL;
    if (!list_get(wNode->page, (element_t)&docID, (element_t)&dNode)) {
      // No matching document, so insert a new one
      assert(dNode == NULL);
      dNode = calloc(1, sizeof(DocumentNode));
      dNode->document_id = docID;
      dNode->page_word_frequency = 1;
      list_append(wNode->page, dNode);
    } else {
      assert(dNode != NULL);
      assert(dNode->document_id == docID);
      dNode->page_word_frequency++;
    }
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
int dNode_cmp(const element_t av, const element_t bv)
{
  DocumentNode* b = bv;
  int* a = (int*)av;
  return b->document_id == *a;
}



static void wNode_concat(WordNode* wNode, char** str)
{
  // TODO strcat slow!
  char* docs = calloc(1,BUF_SIZE);
  sprintf(docs, "%s %d", wNode->word, wNode->page->length);

  ListNode* node = wNode->page->head;
  DocumentNode* dNode;
  while (node) {
    dNode = node->data;
    sprintf(eos(docs), " %d %d", dNode->document_id, dNode->page_word_frequency);
    node = node->next;
  }
  //strcat(docs, "\n");
  if (strlen(docs) + strlen(*str) - 1 >= strlen(*str)) {
    *str = realloc(*str, 2*(strlen(*str)));
  }
  strcat(docs, "\n");
  strcat(*str, docs);
  free(docs);

}

/*
* IndexLoadWords - load hashtable values into string buffer. Each word
* is loaded followed by (1) number indicating how many documents the word is in;
* (2) (a,b) pairs where the document a has b occurences of the word
* @ht: Hashtable to load from
* @buf: pointer to C-style string to loads values
*
* Returns size of buffer
*/
void IndexLoadWords(HashTable* ht, char** buf) 
{ // TODO - this is bugging out
  WordNode* wNode;
  for (int i = 0; i < MAX_HASH_SLOT; i++) { // Go through each hashtable bucket
    HashTableNode* node = ht->table[i];
    while (node) {  // Go through each word node
      if (node == NULL) {
        fprintf(stderr, "ERROR - IndexLoadWords");
        hashtable_destroy(ht);
        free(buf);
        exit(1);
      }
      wNode = node->data;
      wNode_concat(wNode, buf);

      node = node->next;
      
    }
  }
}


/*
* readFile - Read an inverted index in from file and
* construct a new index, i.e. the original.
* @ht: Hashtable to reconstruct
* @filename: Name of file to construct from
*/
void readFile(HashTable* index, char* filename)
{
  FILE* fp;

  fp = fopen(filename, "r");
  ssize_t read;
  size_t len = 0;
  char* line = NULL;
  // Read each line and parse
  while ((read = getline(&line, &len, fp)) != -1) {
    handleLine(index, line);
    /*if (line) {
      free(line);
    }*/
  }
  fclose(fp);
}

/*
* handleLine - Parse line and insert in hashtable
* @index: table in which to insert
* @line: Line to parse
*/
void handleLine(HashTable* index, char* line) {
  char* word;
  char *pch;
  char* saveptr;
  int num_tokens, num_docs, freq;
  int doc_id;
  List* dNodeList;
  DocumentNode* dNode;
  WordNode* wNode;

  // Initialize list 
  num_tokens = 1;
  dNodeList = calloc(1, sizeof(List));
  list_new(dNodeList, sizeof(DocumentNode), dNode_cmp, dNode_free);

  pch = strtok_r(line, " ", &saveptr);
  while (pch != NULL) {
    if (num_tokens == 1) {
      // First token is the word
      word = calloc(1, WORD_LENGTH);
      strcpy(word, pch);

    }
    else if (num_tokens == 2) {
      // Second token is number of DocumentNodes 
      num_docs = atoi(pch);
    }
    else {
      if (num_tokens % 2 == 1) {
        // Odd numbered are document ids
        doc_id = atoi(pch);
      }
      else if (num_tokens % 2 == 0) {
        // Even numbered are number of occurences in doc
        //forms a pair (a,b) we add as a DocumentNode 
        freq = atoi(pch);

        /* Build DocumentNode */
        dNode = calloc(1, sizeof(DocumentNode));
        dNode->document_id = doc_id;
        dNode->page_word_frequency = freq;

        // Construct list of DocumentNodes 
        list_append(dNodeList, dNode);

      }
    }
    num_tokens++;
    pch = strtok_r(NULL, " \n", &saveptr);
  }
  // construct WordNode and add to index 
  wNode = calloc(1, sizeof(WordNode));
  wNode->word = calloc(1, strlen(word)+1);
  strcpy(wNode->word, word);
  wNode->page = dNodeList;
  hashtable_insert(index, wNode->word, wNode);
  free(word);
}

/*
* wNode_cmp - Helper function to compare WordNodes
* in a hashtable of WordNodes
* @elemv: Assume to be the word to match against
* @wNodev: Assume to be the WordNode to match
*/
int wNode_cmp(const element_t av, const element_t bv)
{
  char* a = av;
  char* b = bv;
  return strcmp(a, b) == 0;

}

/*
* wNode_hash - Hashing function to hash WordNodes
*/
uint32_t wNode_hash(const element_t keyv)
{
  char* key = keyv;
  size_t len = strlen(key);

  uint32_t hash = 0;
  uint32_t i = 0;

  for (hash = i = 0; i < len; ++i) {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

/*
* wNode_free - Helper to free WordNodes
*/
void wNode_free(element_t data)
{
  WordNode* wNode = data;
  free(wNode->word);
  wNode->word = NULL;
  list_destroy(wNode->page);
}