CC = gcc
CFLAGS = -Wall -pedantic -ggdb -std=c99
EXEC = crawler
SOURCES=./crawler.c ./crawler.h
CFILES=./crawler.c ./hashtable.c

UTILDIR=../util/
UTILFLAG=-ltseutil
UTILLIB=$(UTILDIR)libtseutil.a
UTILC=$(UTILDIR)hash.c $(UTILDIR)html.c $(UTILDIR)input.c
UTILH=$(UTILC:.c=.h)

$(EXEC):$(SOURCES) $(UTILLIB)
	$(CC) $(CFLAGS) -o $@ $(CFILES) -L$(UTILDIR) $(UTILFLAG)

$(UTILLIB): $(UTILC) $(UTILH)
	cd $(UTILDIR); make; 

clean:
	rm -f *.o
	rm -f $(EXEC)
	rm -f *.gch
