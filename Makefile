
# CC := mpicc

# PSRS : mpiVersion.o
# 	$(CC) -o mpiVersion mpiVersion.o

# PSRS.o : PSRS.c
# 	$(CC) -c mpiVersion.c -o mpiVersion.o

# clean :
# 	-rm -f mpiVersion *.o

CC := mpicc
GCC := gcc

all: lomuto hoare sequential PSRS ompV hybrid

lomuto: lomuto.o
	$(GCC) -o lomuto lomuto.o

lomuto.o: sortingLomuto.c
	$(GCC) -c sortingLomuto.c -o lomuto.o

hoare: hoare.o
	$(GCC) -o hoare hoare.o

hoare.o: sortingHoare.c
	$(GCC) -c sortingHoare.c -o hoare.o

sequential: sorting.o
	$(GCC) -o sorting sorting.o 

sorting.o: sorting.c
	$(GCC) -c sorting.c -o sorting.o 

PSRS: mpiVersion.o
	$(CC) -o mpiVersion mpiVersion.o 

mpiVersion.o: mpiVersion.c
	$(CC) -c mpiVersion.c -o mpiVersion.o 

ompV: openmpVersion.o
	$(GCC) -o openmpVersion openmpVersion.o -fopenmp -lm

openmpVersion.o: openmpVersion.c
	$(GCC) -c openmpVersion.c -o openmpVersion.o -fopenmp -lm

hybrid: hybridVT.o
	$(CC) -o hybridVT hybridVT.o -fopenmp -lm

hybridVT.o: hybridVT.c
	$(CC) -c hybridVT.c -o hybridVT.o -fopenmp -lm
	
clean:
	-rm -f lomuto hoare sorting hybridVT mpiVersion openmpVersion *.o


