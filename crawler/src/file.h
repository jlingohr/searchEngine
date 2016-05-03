#ifndef _FILE_H
#define _FILE_H

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

void unix_error(char* msg);

void Stat(const char* filename, struct stat* buf);

#endif