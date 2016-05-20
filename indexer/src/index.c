#include "index.h"

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
* IndexAddWord - Add word to hashtable index
* @ht: Hashtable adding to
* @word: Word we are adding to the table
* @docID: document id for word
*
* Returns 1 on success
*/
int IndexAddWord(HashTable* ht, char* word, int docID) {
  WordNode* wNode;

  wNode = initWNode(word, docID);
  return HashTableAdd(ht, wNode, HashWNode);
}

/*
* IndexAddWNode - Adds a complete WordNode to the table
* @ht: table to add to
* @wNode: WordNode adding
*
*/
void IndexAddWNode(HashTable* ht, WordNode* wNode) {
  HashTableAdd(ht, wNode, HashWNode);
}


/*
* IndexLookUp - Searches for word in hashtable index
* @ht: Table we are looking in
* @word: Word searching for
* @wNode: Saves pointer to WordNode if held
*
* Returns 1 if word is in table
* Returns 0 otherwise
*/
int IndexLookUp(HashTable* ht, char* word) {
  return HashTableLookUp(ht, word, HashString, cmpWNode);
}

/* 
* IndexGet - Return pointer to WordNode corresponding to
* word.
* @ht: Hashtable to retrieve from
* @word: Word we want to retrive
*
* Returns a pointer to a WordNode if found
* Returns NULL if word not contained 
*/
WordNode* IndexGet(HashTable* ht, char* word) {
  /* TODO - check casting is correct */
  WordNode* tmp;

  tmp = (WordNode*)HashTableGet(ht, word, HashString, cmpWNode);
  if (tmp == NULL)
    return NULL;
  return tmp;
}

/*
* IndexUpdate - Updates word frequency count on relevant doc
* @ht: Hashtable to look in
* @word: Word to update
* @docID: Document id to update
*
* Returns 1 on success 
*/
int IndexUpdate(HashTable* ht, char* word, int docID) {
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
* IndexLoadWords - load hashtable values into string buffer. Each word
* is loaded followed by (1) number indicating how many documents the word is in;
* (2) (a,b) pairs where the document a has b occurences of the word
* @ht: Hashtable to load from
* @buf: pointer to C-style string to loads values
*
* Returns size of buffer
*/
int IndexLoadWords(HashTable* ht, char** buf) {
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

      if (strlen(*buf) + strlen(word_buf) - 1 <= sizeof(*buf))
        buf = realloc(buf, 2*sizeof(buf));
      strcat(*buf, word_buf);
      strcat(*buf, "\n");
      free(word_buf);

      node = node->next;      
    }
  }
  return strlen(*buf);
}


/*
* readFile - Read an inverted index in from file and
* construct a new index, i.e. the original.
* @filename: Name of file to construct from
*
* Returns pointer to a hashtable/index
*/
HashTable* readFile(char* filename) {
  /* TODO */
  HashTable* index;
  FILE* fp;
  char *buf;
  int size;

  /* Initialize new HashTable */
  index = initHashTable();
  size = BUF_SIZE;
  buf = (char*)malloc(size);

  fp = fopen(filename, "r");
  ssize_t read;
  size_t len = 0;
  char* line = NULL;
  /* Read each line and parse */
  while ((read = getline(&line, &len, fp)) != -1) {
    handleLine(index, line);
  }
  if (line)
    free(line);

  fclose(fp);
  return index;
}

/*
* handleLine - Parse line and insert in hashtable
* @index: table in which to insert
* @line: Line to parse
*/
void handleLine(HashTable* index, char* line) {
  /* TODO - Buggy doc_id from conversion */
  char word[BUF_SIZE];
  char *pch;
  int num_tokens, num_docs, doc_id, freq;
  List* dNodeList;
  DocumentNode* dNode;
  WordNode* wNode;

  /* Initialize list */
  num_tokens = 1;
  dNodeList = initList();

  pch = strtok(line, " ");
  while (pch != NULL) {
    if (num_tokens == 1) {
      /* First token is the word */
      strcpy(word, pch);
      //printf("word: %s, ", word);
      if (IndexLookUp(index, word))
        return;
    }
    else if (num_tokens == 2) {
      /* Second token is number of DocumentNodes */
      num_docs = atoi(pch);
      //printf("no_docs: %d, ", num_docs);
    }
    else {
      if (num_tokens % 2 == 1) {
        /* Odd numbered are document ids */
        /* This is buggy */
        doc_id = atoi(pch);
        //printf("doc_id: %d, ", doc_id);
      }
      else if (num_tokens % 2 == 0) {
        /* Even numbered are number of occurences in doc
        forms a pair (a,b) we add as a DocumentNode */
        freq = atoi(pch);

        /* Build DocumentNode */
        dNode = malloc(sizeof(DocumentNode));
        dNode->document_id = doc_id;
        dNode->page_word_frequency = freq;

        /* Construct list of DocumentNodes */
        listAddDoc(dNodeList, dNode);
        //printf("freq: %d\n", freq);

      }
    }
    num_tokens++;
    pch = strtok(NULL, " ");
  }
  /* construct WordNode and add to index */
  wNode = malloc(sizeof(WordNode));
  strcpy(wNode->word, word);
  wNode->page = dNodeList;
  IndexAddWNode(index, wNode);

  
}

/*
* cleanIndex - Cleans up memory for hashtable
* of WordNodes
* @ht: table to clean
*/
void cleanIndex(HashTable* ht) {
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

