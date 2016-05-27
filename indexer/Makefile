CC=gcc
CFLAGS = -Wall -std=gnu11
PATH = ./src/
UTIL = ../util/
TARGET = ./bin/

LIBFLAGS = -lcurl
EXEC = indexer
OBJS = $(PATH)index.o $(UTIL)hashtable.o $(UTIL)web.o $(UTIL)util.o $(UTIL)list.o
SRC = $(PATH)indexer.c $(PATH)index.c $(UTIL)hashtable.c $(UTIL)web.c $(UTIL)util.c $(UTIL)list.c

indexer: $(SRC)
	$(CC) $(CFLAGS) -g -o $(TARGET)indexer $(SRC) $(LIBFLAGS)



clean: 
	rm -f *.o