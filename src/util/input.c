#include "input.h"
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <ctype.h>

char* isValidSeed(char* seed) {
	return seed;
}

void isValidDir(char* dir) {
	struct stat statbuf;
	if (stat(dir, &statbuf) == -1) {
		perror("Invalid directory.");
		exit(1);
	}
	if (!S_ISDIR(statbuf.st_mode)) {
		perror("No directory given.");
		exit(1);
	}
	if ((statbuf.st_mode & S_IWUSR) != S_IWUSR) {
		perror("Directory not writable");
		exit(1);
	}
}
