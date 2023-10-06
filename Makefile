CFLAGS = -lm -Wall -g
CC = gcc
objects = ajustePol.o polynomial_ops.o linear_ops.o utils.o

all: ajustePol

ajustePol: $(objects)
	$(CC) $(objects) -o ajustePol $(CFLAGS)

ajustePol.o: ajustePol.c polynomial_ops.h
	$(CC) -c ajustePol.c $(CFLAGS)

polynomial_ops.o: polynomial_ops.c polynomial_ops.h
	$(CC) -c polynomial_ops.c $(CFLAGS)

linear_ops.o: linear_ops.c linear_ops.h
	$(CC) -c linear_ops.c $(CFLAGS)

utils.o: utils.c utils.h
	$(CC) -c utils.c $(CFLAGS)

clean:
	-rm -f $(objects)

purge: clean
	-rm -f ajustePol

clear:
	clear

exec: ajustePol
	./ajustePol

rebuild: purge all clear exec