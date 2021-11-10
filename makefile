CC = gcc
CFLAGS = -Wall
ARQS = mosaico.c lib_mosaico.c

all:
	$(CC) $(CFLAGS) -c $(ARQS)
	$(CC) $(CFLAGS) -o mosaico *.o -lm

clean:
	rm -f *.o

purge: clean
	rm -f mosaico