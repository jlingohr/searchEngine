#include "file.h"

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