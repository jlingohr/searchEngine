CFLAGS=-g -O2 -Wall -Wextra -DNDEBUG
CC=gcc

all: list_test hashtable_test indexer_test

list_test: ../util/list.c ../util/list.h ./src/list_test.c
	$(CC) $(CFLAGS) -o ./bin/list_test ../util/list.c ./src/list_test.c

hashtable_test: ../util/hashtable.c ../util/hashtable.h ./src/hashtable_test.c
	$(CC) $(CFLAGS) -o ./bin/hashtable_test ../util/hashtable.c ./src/hashtable_test.c

indexer_test: ../indexer/src/index.c ../util/list.c ../util/list.h ../indexer/src/index.h ../util/hashtable.c ../util/hashtable.h ./src/indexer_test.c
	$(CC) $(CFLAGS) -o ./bin/indexer_test ../indexer/src/index.c ../util/list.c ../util/hashtable.c ./src/indexer_test.c 

clean:
	rm -rf *.dSYM