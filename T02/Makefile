LIKWID_HOME = /usr/local
CFLAGS = -lm -Wall -g -I$(LIKWID_HOME)/include -DLIKWID_PERFMON -O3 -mavx -march=native
LFLAGS = -L$(LIKWID_HOME)/lib -llikwid
CC = gcc

objects = ajustePol.o system_ops.o interval_ops.o utils.o

EXECUTABLE = ajustePol

all: $(EXECUTABLE) geraEntrada

$(EXECUTABLE): $(objects)
	$(CC) $(objects) $(CFLAGS) -o $(EXECUTABLE) $(LFLAGS)

geraEntrada: gera_entrada.c
	$(CC) $< $(CFLAGS) -o $@

ajustePol.o: ajustePol.c system_ops.h
	$(CC) -c $< $(CFLAGS)

system_ops.o: system_ops.c system_ops.h
	$(CC) -c $< $(CFLAGS)

interval_ops.o: interval_ops.c interval_ops.h
	$(CC) -c $< $(CFLAGS)

utils.o: utils.c utils.h
	$(CC) -c $< $(CFLAGS)

clean:
	-rm -f $(objects)  

purge: clean
	-rm -f $(EXECUTABLE) geraEntrada

clear:
	clear

exec: $(EXECUTABLE)
	./$(EXECUTABLE)

script: script.sh
	./script.sh

rebuild: purge all clear exec