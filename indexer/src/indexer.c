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
int getDocID(char* filename);
int getNextWord(const char* doc, int pos, char** word);
int updateIndex(char* word, int docID, InvertedIndex* index);
int saveIndexToFile(InvertedIndex* index);

HashTable* Index;

int main(int argc, char** argv) {
  char *target_directory, *target_file, *test_old, *test_new;
  char* doc, *prev_file;
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

  /* Prepend directory name to filenames?
  for (int i = 0; i < num_files; i++) {
    printf("%s\n", filenames[i]);
    
  } */
  

  //3. LOG( "Building the index" );
  if (STATUS_LOG == 1) {
    printf("\nBuilding the index\n");
  }

  /*4. Index *wordindex=buildIndexFromDirectory("argv[1]"); */
  for (int i = 0; i < num_files; i++) {
    doc = loadDoc(filenames[i]);
  }


  //5. saveFile (argv[2], wordindex );
    // LOG( "done!");
     
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
  FILE* fp;
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
  //TODO
  return NULL;
}

/*
* getDocID - Generates document identifier from the name of the file. ASSUME
* crawler saved the filed using unique identifiers as names
* @doc: name of the file
*
* Returns integer identifying document
*/
int getDocID(char* filename) {
  //TODO
  return -1;
}

/*
* getNextWord - Parses the string containing the loaded document skippng the HTML tags
* @doc: string containing the HTML document
* @pos: current position within that document (starts at 0)
* @word: pointer to C-style string
*
* Returns the current position in the document and a newly allocated word (must be freed)
*/
int getNextWord(const char* doc, int pos, char** word) {
  // TODO
  return -1;
}

/* updateIndex - Updates the structure containing the index
* @word: string containing the word
* @docID: identifier of the document
* @index: InvertedIndex hashtable
*
* Returns 1 if successful
* Returns 0 otherwise
*/
int updateIndex(char* word, int docID, InvertedIndex* index) {
  //TODO
  return 0;
}

/*
* saveIndexToFile - Saves the inverted index to a file in .dat format
* @index: Pointer to the inverted index
*
* Returns 1 if successful
* Returns 0 otherwise
*/
int saveIndexToFile(InvertedIndex* index) {
  //TODO
  return 0;
}