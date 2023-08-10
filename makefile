CFLAGS = -lm -Wall -g 

CC = gcc

	objects = main.o

all: main.o
	$(CC) $(CFLAGS) main.o -o calc 

main.o: main.c 
	$(CC) $(CFLAGS) main.c -c

clean:
	-rm -f $(objects)

purge: 
	-rm -f $(objects) calc

rebuild: purge all