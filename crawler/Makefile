CC=gcc
CFLAGS = -Wall -std=gnu11
PATH = ./src/

LIBFLAGS = -lcurl
EXEC = crawler
OBJS = $(PATH)hashtable.o $(PATH)list.o $(PATH)web.o $(PATH)file.o
SRC = $(PATH)crawler.c $(PATH)hashtable.c $(PATH)list.c $(PATH)web.c $(PATH)file.c

crawler: $(SRC)
	$(CC) $(CFLAGS) -g -o crawler $(SRC) $(LIBFLAGS)

clean: 
	rm *.o