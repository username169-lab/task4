CC=gcc
CFLAGS=-c -Wall -g -I.
all: ls wc

ls: ls.o
	$(CC) ls.o -o ls

ls.o: ls.c
	$(CC) $(CFLAGS) ls.c

wc: wc.o
	$(CC) wc.o -o wc

wc.o: wc.c
	$(CC) $(CFLAGS) wc.c

clean:
	rm -rf *.o ls wc
