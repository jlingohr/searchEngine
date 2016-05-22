CC=gcc
CFLAGS = -Wall -std=gnu11
UTIL = ../

LIBFLAGS = -lcurl
EXEC = test_list test_hash
OBJSL = test_list.o $(UTIL)util.o $(UTIL)list.o
SRCL = test_list.c $(UTIL)util.c $(UTIL)list.c

OBJSH = test_hash.o $(UTIL)util.o $(UTIL)hashtable.o
SRCH = test_hash.c $(UTIL)util.c $(UTIL)hashtable.c

all: test_list test_hash

test_list: $(SRCL)
	$(CC) $(CFLAGS) -g -o test_list $(SRCL) $(LIBFLAGS)

test_hash: $(SRCH)
	$(CC) $(CFLAGS) -g -o test_hash $(SRCH) $(LIBFLAGS)

clean: 
	rm -f *.o
	rm -r test.dSYM