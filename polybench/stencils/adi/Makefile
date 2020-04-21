CC=mpicc
POLY_DIR=/home/caos/jalcaraz/polybench-c-4.2.1-beta-OPENMP/
CFLAGS=-I${POLY_DIR}/utilities -lm -fopenmp -O2
CURRENT=$(shell pwd)
BENCHMARK=$(shell basename $(CURRENT))


default: $(BENCHMARK).o
	$(CC) $(CFLAGS) $(BENCHMARK).o -o $(BENCHMARK)  ${POLY_DIR}/utilities/polybench.c
clean:
	rm $(BENCHMARK) $(BENCHMARK).o

