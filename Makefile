CFLAGS = -lm -Wall -g
CC = gcc
objects = main.o interval_operations.o

all: calc

calc: $(objects)
	$(CC) $(objects) -o calc $(CFLAGS)

main.o: main.c interval_operations.h
	$(CC) -c main.c $(CFLAGS)

interval_operations.o: interval_operations.c interval_operations.h
	$(CC) -c interval_operations.c $(CFLAGS)

clean:
	-rm -f $(objects)

purge: clean
	-rm -f calc

clear:
	clear

exec: calc
	./calc

rebuild: purge all clear exec
