CFLAGS = -lm -Wall -g 

CC = gcc

	objects = main.o

all: main.o
	$(CC) main.o -o calc $(CFLAGS)

main.o: main.c 
	$(CC) main.c -c $(CFLAGS) 

clean:
	-rm -f $(objects)

purge: 
	-rm -f $(objects) calc

rebuild: purge all