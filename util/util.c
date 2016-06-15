#include "util.h"



#define _BSD_SOURCE


/* Private Prototypes */
static int SelectFiles(const struct dirent* entry);

/* Public Functions */


/****************
* Wrappers to read/write
*****************/

void unix_error(char* msg) {
  fprintf(stderr, "%s: %s\n", msg, strerror(errno));
  exit(0);
}

void Stat(const char* filename, struct stat* buf) {
  if (stat(filename, buf) < 0)
    unix_error("Stat error");
}


void Fputs(const char* ptr, FILE* stream) {
  if (fputs(ptr, stream) == EOF)
    unix_error("Fputs error");
}

FILE* Fopen(const char* filename, const char* mode) {
  FILE* fp;
  if ((fp = fopen(filename, mode)) == NULL)
    unix_error("Fopen error");

  return fp;
}

void Fclose(FILE* fp) {
  if (fclose(fp) != 0)
    unix_error("Fclose error");
}

/*****************
* Procedures to work with files
*****************/

/*
* isDirec - Checks if target directory is valid directory
* and whether directory is writable
* Returns 1 if true
* else returns 0
*/
int isDirec(const char* dir) {
  struct stat sbuf;
  if (!dir)
    return 0;

  Stat(dir, &sbuf);
  if (!S_ISDIR(sbuf.st_mode)) {
    printf("Target %s is not a directory.\n", dir);
    return 0;
  }
  if (!(sbuf.st_mode & S_IWUSR)) {
    printf("%s is not writable.\n", dir);
    return 0;
  }
  if (!(sbuf.st_mode & S_IRUSR)) {
    printf("%s is not readable.\n", dir);
    return 0;
  }
  return 1;
}

/*
* Determine if path is a file
* Return non-zero if true
* Return 0 otherwise
*/
int isFile(const char* path) {
  struct stat sbuf;
  
  if (!path)
    return 0;

  return (!stat(path, &sbuf) && S_ISREG(sbuf.st_mode));
}

/*
* isDir - Determines if path is a directory as used by INDEX
* @path: path to check
*
* Returns nonzero if path is a directory
* Returns 0 otherwise
*/
int isDir(const char *path) {
  struct stat sbuf;

  if (!path) return 0;

  return (!stat(path, &sbuf) && S_ISDIR(sbuf.st_mode) && (sbuf.st_mode & S_IRUSR));
}

/*
* getFilenamesInDir - Populate a list of filenames found in dir
* @dir: directory path to search
* @filenames: pointer to an array of c-style strings
*
* Returns the number of filenames found; otherwise, a negative number indicates
* failure. The filenames array should not be preallocated, but will be created
* by this function. It is the caller's responsibility to free the filenames and
* the array containing them.

*/
int getFileNamesInDir(const char* dir, char*** filenames) {
  //TODO
  struct dirent **namelist;                // list of dir entries from scandir
  int num_ents;                            // number of regular files found
  int status;                              // return status

  // make sure we have a directory path to work with
  if(!dir) { return -1; }

  // scan the directory for regular files
  status = num_ents = scandir(dir, &namelist, SelectFiles, alphasort);
  if(num_ents < 0) {                       // something went wrong scanning
      return -1;
  }

  // allocate the filenames array
  *filenames = calloc(num_ents, sizeof(char*));
  if(!*filenames) {
      status = -1;
  }

  // copy all the filenames and free the dirents
  for(int i = 0; i < num_ents; i++) {
      // allocate a filename large enough to contain the name
      (*filenames)[i] = calloc(strlen(namelist[i]->d_name) + 1, sizeof(char));
      if(!(*filenames)[i]) {
          status = -1;
      }

      // copy the filename
      strcpy((*filenames)[i], namelist[i]->d_name);
  }

  if(namelist) {
      for(int i = 0; i < num_ents; i++) {
          if(namelist[i])
              free(namelist[i]);
      }
      free(namelist);
  }

  return status;
}

/*
* SelectFiles - Test if a directory entry is a file
* Returns nonzero value if true
* Returns 0 otherwise
*/
static int SelectFiles(const struct dirent* entry) {
  return (entry->d_type == DT_REG || entry->d_type == DT_UNKNOWN);
}

/*
* Signal - wrapper for sigaction
*/
handler_t* Signal(int signum, handler_t* handler)
{
  struct sigaction action, old_action;
  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = SA_RESTART;

  if (sigaction(signum, &action, &old_action) < 0) {
    unix_error("Signal error");
  }
  return (old_action.sa_handler);
}

/*
* Fork - Wrapper for fork
*/
pid_t Fork()
{
  pid_t pid;

  if ((pid = fork()) < 0) {
    unix_error("Fork error");
  } 
  return pid;
}