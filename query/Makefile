CC=gcc
CFLAGS = -Wall -std=gnu11
PATH = ./src/
UTIL = ../util/
TARGET = ./bin/
INDEX = ../indexer/src/

LIBFLAGS = -lcurl
EXEC = query
OBJS = $(PATH)query.o $(PATH)parse.o $(PATH)rank.o $(INDEX)index.o $(UTIL)hashtable.o $(UTIL)web.o $(UTIL)util.o $(UTIL)list.o
SRC = $(PATH)query.c $(PATH)parse.c $(PATH)rank.c $(INDEX)index.c $(UTIL)hashtable.c $(UTIL)web.c $(UTIL)util.c $(UTIL)list.c

query: $(SRC)
	$(CC) $(CFLAGS) -g -o $(TARGET)query $(SRC) $(LIBFLAGS)

clean: 
	rm -f *.o