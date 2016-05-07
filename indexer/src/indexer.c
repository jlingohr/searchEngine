/*
* Default use:
* ./indexer  [TARGET_DIRECTORY] [RESULTS FILE NAME]
* 
* For testing ability to reconstruct the inverted index
* ./indexer  [TARGET_DIRECTORY] [RESULTS FILENAME] [RESULTS FILENAME] [REWRITEN FILENAME]
*
* To run: ./index ../crawler/data/ ./data/index.dat
*/

#include <stdio.h>                           // printf
#include <sys/stat.h>                        // isdir
#include <sys/types.h>
#include <unistd.h>
#include <curl/curl.h>                       // curl functionality
#include <string.h>                          // strncmpr

// ---------------- Local includes  e.g., "file.h"
#include "../../util/hashtable.h"                       // hashtable functionality
#include "../../util/common.h"                          // common functionality
#include "../../util/util.h"
#include "../../util/web.h"                             // curl and html functionality



/* Private Prototypes */
int checkCommandLine(int argc, char** argv);
char* loadDoc(char* filename);
int getDocID(char* filename, char* dir);
int updateIndex(char* word, int docID, HashTable* index);
int saveIndexToFile(HashTable* index);
void initWNode(WordNode* wNode, char* word, int docID);
int updateDNode(DocumentNode* dNode, int docID);
void initDNode(DocumentNode* dNode, int docID);

HashTable* Index;

int main(int argc, char** argv) {
  char *target_directory, *target_file, *test_old, *test_new;
  char* prev_file;
  char** filenames;
  int num_files;
  

  /*1. Program parameter processing */
  if (!checkCommandLine(argc, argv)) {
    fprintf(stderr, "usage: <target> <result file>\n");
    exit(1);
  }

  /*2. Initialize data structures
       allocate Inverted_index, zero it, and set links to NULL. */
  Index = initHashTable();

  target_directory = argv[1];
  target_file = argv[2];
  if (argc == 5) {
    test_old = argv[3];
    test_new = argv[4];
  }
  num_files = getFileNamesInDir(target_directory, &filenames);
  if (num_files < 0) {
    fprintf(stderr, "Failure reading filenames.\n");
    /* TODO - Free memory */
    return 1;
  }

  /* Prepend directory name to filenames */
  for (int i = 0; i < num_files; i++) {
    char* tmp = filenames[i];
    int len = strlen(target_directory) + strlen(tmp) + 1;
    char* buf = calloc(len, sizeof(char));
    strcpy(buf, target_directory);
    strcat(buf, tmp);
    if (tmp)
      free(tmp);

    filenames[i] = buf;
  }
  

  //3. LOG( "Building the index" );
  if (STATUS_LOG == 1) {
    printf("\nBuilding the index\n");
  }

  /*4. Index *wordindex=buildIndexFromDirectory("argv[1]"); */
  char* doc, *word;
  int doc_id, pos;
  for (int i = 0; i < num_files; i++) {
    if (!isFile(filenames[i])) {
      fprintf(stderr, "%s not a file\n", filenames[i]);
      continue;
    }
    doc = loadDoc(filenames[i]);
    doc_id = getDocID(filenames[i], target_directory);
    pos = 0;

    while ((pos = GetNextWord(doc, pos, &word)) > 0)
      updateIndex(word, doc_id, Index);
  }


  //5. saveFile (argv[2], wordindex );
    // LOG( "done!");
  if (STATUS_LOG == 1) {
    printf("\nLogging complete!\n");
  }
     
  //6. CleanDynamicList (wordindex)

  // if testing then proceed

  //7.  If (5 == argc) then 

     //8. LOG ("Testing index\n");

     //9. Reload the index from the file and rewrite it to a new file
       // wordindex = readFile(argv[3]);

    //10. saveFile (argv[4], wordindex);
       // LOG("test complete\n");

    //11. CleanDynamicList (wordindex);

  //12. Done.

  return 0; 
}

/*
* checkCommandLine - Checks whether valid arguments passed
* on the command line:
* Returns 1 argc == (3|5) && directory and files are valid
* Returns 0 otherwise
*/
int checkCommandLine(int argc, char** argv) {
  char* dir, *file;
  int index;

  if (argc != 3 && argc != 5)
    return 0;

  dir = argv[1];
  file = argv[2];

  /* Validate directory */
  index = strlen(dir)-1;
  if (dir[index] != '/') {
    dir = malloc(strlen(argv[1]) + 1); // Dont forget to free
    strcpy(dir, argv[1]);
    strcat(dir, "/");
  }
  //printf("Checking directory: %s\n", dir);
  if (!isDir(dir))
    return 0;

  /* Validate file */
  if (isFile(file)) {
    fprintf(stderr, "File %s already exists\n", file);
    return 0;
  }

  if (argc == 5) { /* For testing */
    char* old;
    char* new;

    old = argv[3];
    new = argv[4];

    if (strcmp(file, old) != 0) {
      fprintf(stderr, "files must be the same\n");
      return 0;
    }

    if (isFile(new)) {
      fprintf(stderr, "File %s already exists\n", new);
      return 0;
    }

  }

  return 1;

}

/*
* loadDoc - Loads the HTML document from a file
* @filename: name of file to be loaded
*
* Returns string containing the loaded document
*/
char* loadDoc(char* filename) {
  /* TODO - Better way to prevent leaks/dangling pointers */
  char* buf;
  FILE* fp;
  long bufsize;

  buf = NULL;
  fp = fopen(filename, "r");
  if (fp) {
    /* Go to end of the file */
    if (fseek(fp, 0l, SEEK_END) == 0) {
      /* Get size of file */
      bufsize = ftell(fp);
      if (bufsize == -1) {
        fclose(fp);
        return NULL;
      }

      /* Allocate buffer to that size */
      buf = malloc(sizeof(char) * (bufsize + 1));

      /* Go back to start */
      if (fseek(fp, 0l, SEEK_SET) != 0) {
        fclose(fp);
        return NULL;
      }

      /* Read file into buf */
      size_t new_len = fread(buf, sizeof(char), bufsize, fp);
      if (new_len == 0) {
        fprintf(stderr, "Error reading file %s\n", filename);
        fclose(fp);
        return NULL;
      }
      else {
        buf[new_len++] = '\0';
      }
    }
  }
  fclose(fp);
  return buf;
}

/*
* getDocID - Generates document identifier from the name of the file. ASSUME
* crawler saved the filed using unique identifiers as names
* @doc: name of the file
* @dir: Directory file is in
*
* Returns integer identifying document
*/
int getDocID(char* filename, char* dir) {
  //TODO
  int pos;
  char* id;

  pos = strlen(dir);
  id = filename + pos;
  return atoi(id);
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
int updateIndex(char* word, int docID, HashTable* index) {
  //TODO
  WordNode* wNode;

  if (!HashTableLookUpWord(index, word, &wNode)) { /* Word is not in index */
    wNode = malloc(sizeof(WordNode));
    initWNode(wNode, word, docID);
    return HashTableAddWord(index, word);
  }
  /* Word is already in index */
  return updateDNode(wNode->page, docID);
}

/*
* saveIndexToFile - Saves the inverted index to a file in .dat format
* @index: Pointer to the inverted index
*
* Returns 1 if successful
* Returns 0 otherwise
*/
int saveIndexToFile(HashTable* index) {
  //TODO
  return 0;
}

/**************
* Helpers 
**************/

/*
* initDNode - allocates DocumentNode and assigns initial values
* @dNode: DocumentNode to be initialized
* @docID: document id
*
*/
void initDNode(DocumentNode* dNode, int docID) {
  dNode->document_id = docID;
  dNode->page_word_frequency = 1;
  dNode->next = NULL;
}

/*
* initWNode - Initializes WordNode and assigns initial values
* @wNode: WordNote to initialize 
* @word: Word the node corresponds to
* @docID: Document ID for new WordNode
*
*/
void initWNode(WordNode* wNode, char* word, int docID) {
  /* TODO - BUGS here with assigning dNOde to list;
  Check for memory leaks */
  DocumentNode* dNode; 

  /* copy over word */
  strcpy(wNode->word, word);

  /* Allocate DocumentNode */
  dNode = malloc(sizeof(DocumentNode));
  initDNode(dNode, docID);
  wNode->page = dNode;
}

/*
* updateDNode - Updates the occurence of word in the
* document docID
* @dNodeList: List of documents for the word
* @docID: Document ID
*
* Returns 1 on success
*/
int updateDNode(DocumentNode* dNode, int docID) {
  /* TODO - Decouple! */
  DocumentNode* cur;
  DocumentNode* next;

  cur = dNode;
  /* Find last document */
  while ((next = cur->next)) {
    cur = next;
  }
  /* Check document ID */
  if (cur->document_id != docID) { /* Create new DocumentNode */
    DocumentNode* node = malloc(sizeof(DocumentNode));
    initDNode(node, docID);
  }
  else
    cur->page_word_frequency++;

  return 1;

}