/*
* Default use:
* ./indexer  [TARGET_DIRECTORY] [RESULTS FILE NAME]
* 
* For testing ability to reconstruct the inverted index
* ./indexer  [TARGET_DIRECTORY] [RESULTS FILENAME] [RESULTS FILENAME] [REWRITEN FILENAME]
*
* To run: ./index ../crawler/data/ ./data/index.dat
*
* To Test: ./index ../crawler/data/ ./data/index.dat ./data/index.dat ./data/newindex.dat
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
int saveIndexToFile(char* file, HashTable* index);
void cleanIndex(HashTable* Index);
HashTable* readFile(char* filename);
void handleLine(HashTable* index, char* line);



int main(int argc, char** argv) {
  HashTable* Index;

  char *target_directory, *target_file, *test_old, *test_new;
  char* prev_file;
  char** filenames;
  int num_files, saved;
  

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

    while ((pos = GetNextWord(doc, pos, &word)) > 0) {
      NormalizeWord(word);
      updateIndex(word, doc_id, Index);
    }
    free(doc);
  }


  //5. saveFile (argv[2], wordindex );
    // LOG( "done!");
  if (STATUS_LOG == 1) {
    printf("\nLogging complete!\n");
    //HashTablePrintWords(Index);
  }

  saved = saveIndexToFile(target_file, Index);
  while (! saved) {;}
     
  //6. CleanDynamicList (wordindex)
  cleanIndex(Index);

  /* For testing (argc == 5) */
  if (argc == 5) {
    //LOG(argv[3]);
    printf("Rebuilding index...\n");

    /*7. Reload index from file and rewrite to new file
      wordindex = readFile(argv[3]) */
    Index = readFile(argv[3]);

    /*8. saveFile (argv[4]. wordindex) */
    //LOG("Test complete\n");
    saveIndexToFile(argv[4], Index);
    printf("Test complete\n");

    /*9. cleanDynamicList(wordindex) */
    cleanIndex(Index);
  }
  //free(Index);


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

  if (HashTableLookUpWord(index, word)) { /* Word is in table */
    /* Update docID for word */
    return HashTableUpdateWord(index, word, docID);
  }
  else {
    /* Word not in, so insert word */
    return HashTableAddWord(index, word, docID);
  }
  return 1;
}

/*
* saveIndexToFile - Saves the inverted index to a file in .dat format
* @index: Pointer to the inverted index
*
* Returns 1 if successful
* Returns 0 otherwise
*/
int saveIndexToFile(char* file, HashTable* index) {
  /* TODO - Works except printing some html! */
  FILE* fp;
  char* buf;
  int size;

  buf = malloc(BUF_SIZE);
  size = HashTableLoadWords(index, &buf);

  fp = fopen(file, "w+");
  if (fp) {
    Fputs(buf, fp);
    fclose(fp);
    return 1;
  }
  if (fp == NULL) {
    fprintf(stderr, "Error opening file\n");
    return 0;
  }
  return 0;
}

/*
* cleanIndex - Cleans up memory
* @index: table to clean
*/
void cleanIndex(HashTable* index) {
  cleanHashWord(index);
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
  char *buf, c;
  int size, pos;

  /* Initialize new HashTable */
  index = initHashTable();
  size = BUF_SIZE;
  buf = (char*)malloc(size);


  if ((fp = fopen(filename, "r"))) {
    /* Read lines in file into a buff */
    do {
      pos = 0;
      do { /* read one line */
        c = fgetc(fp);
        if (c != EOF)
          buf[pos++] = (char)c;
        if (pos >= size - 1) {
          /* Increase buffer size */
          size *= 2;
          buf = (char*)realloc(buf, size);
        }
      } while (c != EOF && c != '\n');
      buf[pos] = 0;
      /* Line now in buf */
      if (buf)
        handleLine(index, buf);
    } while (c != EOF);
    fclose(fp);
  }
  free(buf);
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
      if (HashTableLookUpWord(index, word))
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
  HashTableAddWNode(index, wNode);

  
}
