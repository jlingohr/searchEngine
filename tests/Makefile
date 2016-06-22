CFLAGS=-g -O2 -Wall -Wextra -DNDEBUG
CC=gcc
UTIL = ../util/
LIBFLAGS = -lcurl

#UTILS = ../util/web.c ../util/list.c ../util/hashtable.c

all: list_test hashtable_test indexer_test crawler_test

list_test: ../util/list.c ../util/list.h ./src/list_test.c
	$(CC) $(CFLAGS) -o ./bin/list_test ../util/list.c ./src/list_test.c

hashtable_test: ../util/hashtable.c ../util/hashtable.h ./src/hashtable_test.c
	$(CC) $(CFLAGS) -o ./bin/hashtable_test ../util/hashtable.c ./src/hashtable_test.c

indexer_test: ../indexer/src/index.c $(UTIL)web.c $(UTIL)list.c $(UTIL)hashtable.c ../indexer/src/index.h ./src/indexer_test.c
	$(CC) $(CFLAGS) -o ./bin/indexer_test $(UTIL)web.c $(UTIL)list.c $(UTIL)hashtable.c ../indexer/src/index.c ./src/indexer_test.c $(LIBFLAGS)

crawler_test: ./src/crawler_test.c $(UTIL)list.c $(UTIL)hashtable.c
	$(CC) $(CFLAGS) -o ./bin/crawler_test $(UTIL)list.c $(UTIL)hashtable.c ./src/crawler_test.c $(LIBFLAGS)

clean:
	rm -rf *.dSYM