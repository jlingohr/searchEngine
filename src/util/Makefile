# Makefile for util directory

CC=gcc
CFLAGS= -Wall
SOURCES = *.c

make:
	$(CC) $(CFLAGS) -c $(SOURCES)
	ar -cvq libtseutil.a *.o

clean:
	rm -f *.o
	rm -f libtseutil.a