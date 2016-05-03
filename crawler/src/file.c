#include "file.h"

void unix_error(char* msg) {
  fprintf(stderr, "%s: %s\n", msg, strerror(errno));
  exit(0);
}

void Stat(const char* filename, struct stat* buf) {
  if (stat(filename, buf) < 0)
    unix_error("Stat error");
}

/*
* writen - Write n bytes from usrbuf to fd in thread-safe manner.
* Never returns a short count
*/
ssize_t writen(int fd, void* usrbuf, size_t n) {
  size_t nleft = n;
  ssize_t nwritten;
  char* bufp = usrbuf;

  while (nleft > 0) {
    if ((nwritten = write(fd, bufp, nleft)) <= 0) {
      if (errno == EINTR)
        nwritten = 0;
      else
        return -1;
    }
    nleft -= nwritten;
    bufp += nwritten;
  }
  return n;
}

/*
* Fopen - Wrapper function for open
*/
FILE* Fopen(const char* filename, const char* mode) {
  FILE* fp;
  if ((fp = fopen(filename, mode)) == NULL)
    unix_error("Fopen error");

  return fp;
}

/*
* Fclose - Wrapper function for close
*/
void Fclose(FILE* fp) {
  if (fclose(fp) != 0)
    unix_error("Fclose");
}