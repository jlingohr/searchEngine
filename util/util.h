#ifndef _FILE_H
#define _FILE_H

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

// Print s with the source file name and the current line number to fp.
#define LOG(fp,s)  fprintf((fp), "Log: [%s:%d] %s.\n", __FILE__, __LINE__, (s))

// Print error to fp and exit if s evaluates to false.
#define ASSERT_FAIL(fp,s) do {                                          \
        if(!(s))   {                                                    \
            fprintf((fp), "Error: [%s:%d] assert failed.\n", __FILE__, __LINE__); \
            exit(EXIT_FAILURE);                                         \
        }                                                               \
    } while(0)

// Print error to fp and exit if s is NULL.
#define MALLOC_CHECK(fp,s)  do {                                        \
        if((s) == NULL)   {                                             \
            fprintf((fp), "Error: [%s:%d] out of memory.\n", __FILE__, __LINE__); \
            exit(EXIT_FAILURE);                                         \
        }                                                               \
    } while(0)


/* Error handling */
void unix_error(char* msg);


/* Wrappers to read/write files */
void Stat(const char* filename, struct stat* buf);
FILE* Fopen(const char* filename, const char* mode);
void Fclose(FILE* fp);
void Fputs(const char* ptr, FILE* stream);

/* Helpers working with files */
int isDirec(const char * dir);
int isFile(const char* path);
int isDir(const char *path);
int getFileNamesInDir(const char* dir, char*** filenames);

#endif

