CC = gcc
CFLAGS = -lm -Wall -g -O3 -mavx -march=native

.PHONY: all clean purge

all: labMonteCarlo

labMonteCarlo: labMonteCarlo.o utils.o
	$(CC) $(CFLAGS) -o labMonteCarlo labMonteCarlo.o utils.o -lm -lrt

labMonteCarlo.o: labMonteCarlo.c
	$(CC) $(CFLAGS) -c labMonteCarlo.c

utils.o: utils.c
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm -f *.o

purge: clean
	rm -f labMonteCarlo