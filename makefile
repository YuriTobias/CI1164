CFLAGS = -lm -Wall -g
CC = gcc
objects = main.o

all: perfEG

perfEG: $(objects)
	$(CC) $(objects) -o perfEG $(CFLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS)

clean:
	-rm -f $(objects)

purge: clean
	-rm -f perfEG

clear:
	clear

exec: perfEG
	./perfEG

rebuild: purge all clear exec
