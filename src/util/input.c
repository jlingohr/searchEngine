#include "input.h"
#include <sys/stat.h>

void isValidDir(char* dir) {
	struct stat dirbuf;
	if (stat(dir, &dirbuf) == -1) {
		perror("Invalid directory.");
		exit(1);
	}
	if (dirbuf.)
}
