CFLAGS = -lm -Wall -g
CC = gcc
objects = main.o linear_ops.o utils.o

all: perfEG

perfEG: $(objects)
	$(CC) $(objects) -o perfEG $(CFLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS)

linear_ops.o: linear_ops.c linear_ops.h
	$(CC) -c linear_ops.c $(CFLAGS)

utils.o: utils.c
	$(CC) -c utils.c $(CFLAGS)

clean:
	-rm -f $(objects)

purge: clean
	-rm -f perfEG

clear:
	clear

exec: perfEG
	./perfEG

rebuild: purge all clear exec
