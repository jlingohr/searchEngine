CC=gcc
CFLAGS = -Wall -std=gnu11
PATH = ./src/

LIBFLAGS = -lcurl
EXEC = crawler
OBJS = $(PATH)dict.o $(PATH)file.o $(PATH)hash.o $(PATH)html.o
SRC = $(PATH)crawler.c $(PATH)dict.c $(PATH)hash.c $(PATH)file.c $(PATH)html.c

crawler: $(SRC)
	$(CC) $(CFLAGS) -o crawler $(SRC) $(LIBFLAGS)

clean: 
	rm *.o