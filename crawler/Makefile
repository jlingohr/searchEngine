CC=gcc
CFLAGS = -Wall -std=gnu11
PATH = ./src/
UTIL = ../util/
TARGET = ./bin/

LIBFLAGS = -lcurl
EXEC = crawler
OBJS = $(UTIL)hashtable.o $(UTIL)list.o $(UTIL)web.o $(UTIL)util.o
SRC = $(PATH)crawler.c $(UTIL)hashtable.c $(UTIL)list.c $(UTIL)web.c $(UTIL)util.c

crawler: $(SRC)
	$(CC) $(CFLAGS) -g -o $(TARGET)crawler $(SRC) $(LIBFLAGS)

clean: 
	rm -f *.o