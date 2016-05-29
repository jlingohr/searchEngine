/*
* Default use:
* ./indexer  [TARGET_DIRECTORY] [RESULTS FILE NAME]
* 
* For testing ability to reconstruct the inverted index
* ./indexer  [TARGET_DIRECTORY] [RESULTS FILENAME] [RESULTS FILENAME] [REWRITEN FILENAME]
*
* To run: ./bin/indexer ../crawler/data/ ./data/index.dat
*
* To Test: ./bin/indexer ../crawler/data/ ./data/index.dat ./data/index.dat ./data/newindex.dat
*/

//#include <stdio.h>                           // printf
#include <sys/stat.h>                        // isdir
#include <sys/types.h>
#include <unistd.h>
#include <curl/curl.h>                       // curl functionality
#include <string.h>                          // strncmpr
#include <pthread.h>
#include <assert.h>

// ---------------- Local includes  e.g., "file.h"
//#include "../../util/hashtable.h"                       // hashtable functionality
#include "index.h"
//#include "../../util/common.h"                          // common functionality
#include "../../util/util.h"
#include "../../util/web.h"                             // curl and html functionality



/* Private Prototypes */
int checkCommandLine(int argc, char** argv);
char* loadDoc(char* filename);
int getDocID(char* filename, char* dir);
//int saveIndexToFile(char* file, HashTable* index);
void* saveIndexToFile(void* argsv);
void cleanUp(HashTable* Index);


struct t_block {
  char* file;
  HashTable* ht;
};






/*****
* Main
*******/




int main(int argc, char** argv) {
  HashTable* Index;

  char *target_directory, *target_file, *test_old, *test_new;
  //char* prev_file;
  char** filenames;
  int num_files;// saved;
  pthread_t tid;
  

  /*1. Program parameter processing */
  if (!checkCommandLine(argc, argv)) {
    fprintf(stderr, "usage: <target> <result file>\n");
    exit(1);
  }

  /*2. Initialize data structures
       allocate Inverted_index, zero it, and set links to NULL. */
  //Index = initHashTable();
  Index = malloc(sizeof(HashTable));
  hashtable_new(Index, sizeof(WordNode), wNode_cmp, wNode_hash, wNode_free);

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
      return 1;
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
  

  //5. Save index to file
  FILE* fp;
  char* buf = NULL;
  //printf("Loading words from index...\n");
  pthread_create(&tid, NULL, IndexLoadWords, Index);
  pthread_join(tid, (element_t*)&buf);
  //size = IndexLoadWords(index, &buf);
  fp = fopen(target_file, "w+");
  if (fp) {
    Fputs(buf, fp);
    fclose(fp);
  }
  if (fp == NULL) {
    fprintf(stderr, "Error opening file\n");
    return 0;
  }
  free(buf);

  if (STATUS_LOG == 1) {
    printf("\nLogging complete!\n");
  }
     
  //6. CleanDynamicList (wordindex)
  cleanUp(Index);
  printf("Cleanup complete...\n");

  /* For testing (argc == 5) */
  if (argc == 5) {
    //LOG(argv[3]);
    Index = malloc(sizeof(HashTable));
    printf("Rebuilding index...\n");

    /*7. Reload index from file and rewrite to new file
      wordindex = readFile(argv[3]) */
    hashtable_new(Index, sizeof(WordNode), wNode_cmp, wNode_hash, wNode_free);
    readFile(Index, test_old);

    /*8. saveFile (argv[4]. wordindex) */
    //LOG("Test complete\n");
    FILE* fd;
    buf = NULL;
    //printf("Loading words from index...\n");
    pthread_create(&tid, NULL, IndexLoadWords, Index);
    pthread_join(tid, (element_t*)&buf);
    //size = IndexLoadWords(index, &buf);
    fd = fopen(test_new, "w+");
    if (fp) {
      Fputs(buf, fd);
      fclose(fd);
      return 0;
    }
    if (fd == NULL) {
      fprintf(stderr, "Error opening file\n");
      return 0;
    }

    //saveIndexToFile(test_new, Index);
    printf("Test complete\n");

    /*9. cleanDynamicList(wordindex) */
    cleanUp(Index);
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


/*
* cleanIndex - Cleans up memory
* @index: table to clean
*/
void cleanUp(HashTable* index) {
  hashtable_destroy(index);
}






