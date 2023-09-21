LIKWID_HOME = /usr/local
CFLAGS = -lm -Wall -g -I$(LIKWID_HOME)/include -DLIKWID_PERFMON -O3 -mavx -march=native
LFLAGS = -L$(LIKWID_HOME)/lib -llikwid
CC = gcc

# Lista de objetos
objects = interpola.o interpolation_ops.o utils.o

# Nome do executável
EXECUTABLE = interpola

all: $(EXECUTABLE)

$(EXECUTABLE): $(objects)
	$(CC) $(objects) $(CFLAGS) -o $(EXECUTABLE) $(LFLAGS)

# Regra de compilação para interpola.c
interpola.o: interpola.c
	$(CC) -c $< $(CFLAGS)

# Regra de compilação para interpolation_ops.c
interpolation_ops.o: interpolation_ops.c interpolation_ops.h
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