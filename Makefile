LIKWID_HOME = /usr/local
CFLAGS = -lm -Wall -g -I$(LIKWID_HOME)/include -DLIKWID_PERFMON -O3 -mavx -march=native -fopt-info-vec-optimized 
LFLAGS = -L$(LIKWID_HOME)/lib -llikwid
CC = gcc

objects = ajustePol.o system_ops.o interval_ops.o utils.o 
objectsOpt = ajustePolOpt.o system_ops_opt.o interval_ops_opt.o utils.o

all: ajustePol ajustePolOpt geraEntrada

ajustePol: $(objects)
	$(CC) $(objects) $(CFLAGS) -o ajustePol $(LFLAGS)

ajustePolOpt: $(objectsOpt)
	$(CC) $(objectsOpt) $(CFLAGS) -o ajustePolOpt $(LFLAGS)

geraEntrada: T02/gera_entrada.c
	$(CC) $< $(CFLAGS) -o $@

ajustePol.o: T01/ajustePol.c T01/system_ops.h
	$(CC) -o $@ -c $< $(CFLAGS)

system_ops.o: T01/system_ops.c T01/system_ops.h
	$(CC) -o $@ -c $< $(CFLAGS)

interval_ops.o: T01/interval_ops.c T01/interval_ops.h
	$(CC) -o $@ -c $< $(CFLAGS)

ajustePolOpt.o: T02/ajustePol.c T02/system_ops.h
	$(CC) -o $@ -c $< $(CFLAGS)

system_ops_opt.o: T02/system_ops.c T02/system_ops.h
	$(CC) -o $@ -c $< $(CFLAGS)

interval_ops_opt.o: T02/interval_ops.c T02/interval_ops.h
	$(CC) -o $@ -c $< $(CFLAGS)

utils.o: T02/utils.c T02/utils.h
	$(CC) -c $< $(CFLAGS)

clean:
	-rm -f $(objects) $(objectsOpt)

purge: clean
	-rm -f ajustePol ajustePolOpt geraEntrada

clear:
	clear

rebuild: purge all clear