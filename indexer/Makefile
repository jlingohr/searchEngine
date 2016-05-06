CC=gcc
CFLAGS = -Wall -std=gnu11
PATH = ./src/
UTIL = ../util/

LIBFLAGS = -lcurl
EXEC = index
OBJS = $(UTIL)hashtable.o $(UTIL)web.o $(UTIL)util.o
SRC = $(PATH)indexer.c $(UTIL)hashtable.c $(UTIL)web.c $(UTIL)util.c

index: $(SRC)
	$(CC) $(CFLAGS) -g -o index $(SRC) $(LIBFLAGS)

clean: 
	rm -f *.o