#include "file.h"

void unix_error(char* msg) {
  fprintf(stderr, "%s: %s\n", msg, strerror(errno));
  exit(0);
}

void Stat(const char* filename, struct stat* buf) {
  if (stat(filename, buf) < 0)
    unix_error("Stat error");
}