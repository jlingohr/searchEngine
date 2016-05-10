CC=gcc
CFLAGS = -Wall -std=gnu11
PATH = ./src/
UTIL = ../util/
TARGET = ./bin/

LIBFLAGS = -lcurl
EXEC = index
OBJS = $(PATH)index.o $(PATH)listD.o $(UTIL)hashtable.o $(UTIL)web.o $(UTIL)util.o $(UTIL)list.o
SRC = $(PATH)indexer.c $(PATH)index.c $(PATH)listD.c $(UTIL)hashtable.c $(UTIL)web.c $(UTIL)util.c $(UTIL)list.c

index: $(SRC)
	$(CC) $(CFLAGS) -g -o $(TARGET)index $(SRC) $(LIBFLAGS)

clean: 
	rm -f *.o