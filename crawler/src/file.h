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

void unix_error(char* msg);

void Stat(const char* filename, struct stat* buf);
ssize_t writen(int fd, void* usrbuf, size_t n);
FILE* Fopen(const char* filename, const char* mode);
void Fclose(FILE* fp);


#endif