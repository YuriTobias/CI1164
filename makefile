LIKWID_HOME = /usr/local
CFLAGS = -lm -Wall -g -I$(LIKWID_HOME)/include -DLIKWID_PERFMON -O3 -mavx -march=native
LFLAGS = -L$(LIKWID_HOME)/lib -llikwid
CC = gcc

# Lista de objetos
objects = main.o linear_ops.o utils.o

# Nome do executável
EXECUTABLE = perfEG

all: $(EXECUTABLE)

$(EXECUTABLE): $(objects)
	$(CC) $(objects) $(CFLAGS) -o $(EXECUTABLE) $(LFLAGS)

# Regra de compilação para main.c
main.o: main.c
	$(CC) -c $< $(CFLAGS)

# Regra de compilação para linear_ops.c
linear_ops.o: linear_ops.c linear_ops.h
	$(CC) -c $< $(CFLAGS)

# Regra de compilação para utils.c
utils.o: utils.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f $(objects) $(EXECUTABLE)

purge: clean

clear:
	clear

exec: $(EXECUTABLE)
	./$(EXECUTABLE)

script: script.sh
	./script.sh

rebuild: purge all clear exec
