CC=gcc
CFLAGS = -Wall -std=gnu11
PATH = ./src/
UTIL = ../util/
TARGET = ./bin/
INDEX = ../indexer/src/

LIBFLAGS = -lcurl
EXEC = query
OBJS = $(PATH)query.o $(INDEX)index.o $(INDEX)listD.o $(UTIL)hashtable.o $(UTIL)web.o $(UTIL)util.o $(UTIL)list.o
SRC = $(PATH)query.c $(INDEX)index.c $(INDEX)listD.c $(UTIL)hashtable.c $(UTIL)web.c $(UTIL)util.c $(UTIL)list.c

index: $(SRC)
	$(CC) $(CFLAGS) -g -o $(TARGET)query $(SRC) $(LIBFLAGS)

clean: 
	rm -f *.o