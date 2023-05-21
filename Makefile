
# CC := mpicc

# PSRS : mpiVersion.o
# 	$(CC) -o mpiVersion mpiVersion.o

# PSRS.o : PSRS.c
# 	$(CC) -c mpiVersion.c -o mpiVersion.o

# clean :
# 	-rm -f mpiVersion *.o


CC := mpicc
GCC := gcc

all: PSRS ompV

PSRS: mpiVersion.o
	$(CC) -o mpiVersion mpiVersion.o 

mpiVersion.o: mpiVersion.c
	$(CC) -c mpiVersion.c -o mpiVersion.o 

ompV: openmpVersion.o
	$(GCC) -o openmpVersion openmpVersion.o -fopenmp -lm

openmpVersion.o: openmpVersion.c
	$(GCC) -c openmpVersion.c -o openmpVersion.o -fopenmp -lm
	
clean:
	-rm -f mpiVersion openmpVersion *.o


