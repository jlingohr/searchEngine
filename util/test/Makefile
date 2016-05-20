CC=gcc
CFLAGS = -Wall -std=gnu11
UTIL = ../

LIBFLAGS = -lcurl
EXEC = test
OBJS = test_list.o $(UTIL)util.o $(UTIL)list.o
SRC = test_list.c $(UTIL)util.c $(UTIL)list.c

index: $(SRC)
	$(CC) $(CFLAGS) -g -o $(TARGET)test $(SRC) $(LIBFLAGS)

clean: 
	rm -f *.o
	rm -r test.dSYM