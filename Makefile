LIKWID_HOME = /usr/local
CFLAGS = -lm -Wall -g -I$(LIKWID_HOME)/include -DLIKWID_PERFMON -O3 -mavx -march=native
LFLAGS = -L$(LIKWID_HOME)/lib -llikwid
CC = gcc

objects = ajustePol.o polynomial_ops.o linear_ops.o utils.o

EXECUTABLE = ajustePol

all: $(EXECUTABLE)

$(EXECUTABLE): $(objects)
	$(CC) $(objects) $(CFLAGS) -o $(EXECUTABLE) $(LFLAGS)

ajustePol.o: ajustePol.c polynomial_ops.h
	$(CC) -c $< $(CFLAGS)

polynomial_ops.o: polynomial_ops.c polynomial_ops.h
	$(CC) -c $< $(CFLAGS)

linear_ops.o: linear_ops.c linear_ops.h
	$(CC) -c $< $(CFLAGS)

utils.o: utils.c utils.h
	$(CC) -c $< $(CFLAGS)

clean:
	-rm -f $(objects) 

purge: clean
	-rm -f $(EXECUTABLE)

clear:
	clear

exec: $(EXECUTABLE)
	./$(EXECUTABLE)

script: script.sh
	./script.sh

rebuild: purge all clear exec