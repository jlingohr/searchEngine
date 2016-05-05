CC=gcc
CFLAGS = -Wall -std=gnu11
PATH = ./src/
UTIL = ../util/

LIBFLAGS = -lcurl
EXEC = crawler
OBJS = $(UTIL)hashtable.o $(PATH)list.o $(UTIL)web.o $(UTIL)util.o
SRC = $(PATH)crawler.c $(UTIL)hashtable.c $(PATH)list.c $(UTIL)web.c $(UTIL)util.c

crawler: $(SRC)
	$(CC) $(CFLAGS) -g -o crawler $(SRC) $(LIBFLAGS)

clean: 
	rm -f *.o