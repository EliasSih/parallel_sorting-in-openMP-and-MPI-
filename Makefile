CC := mpicc
GCC := gcc

all: mpiVersion openmpVersion hybridVT sortingHoare sortingLomuto

mpiVersion: mpiVersion.o
	$(CC) -o mpiVersion mpiVersion.o 

mpiVersion.o: mpiVersion.c
	$(CC) -c mpiVersion.c

openmpVersion: openmpVersion.o
	$(GCC) -o openmpVersion openmpVersion.o -fopenmp -lm

openmpVersion.o: openmpVersion.c
	$(GCC) -c openmpVersion.c -fopenmp -lm

hybridVT: hybridVT.o
	$(CC) -o hybridVT hybridVT.o -fopenmp -lm

hybridVT.o: hybridVT.c
	$(CC) -c hybridVT.c -fopenmp -lm

sortingHoare: sortingHoare.o
	$(GCC) -o sortingHoare sortingHoare.o 

sortingHoare.o: sortingHoare.c
	$(GCC) -c sortingHoare.c 

sortingLomuto: sortingLomuto.o
	$(GCC) -o sortingLomuto sortingLomuto.o 

sortingLomuto.o: sortingLomuto.c
	$(GCC) -c sortingLomuto.c 

clean:
	-rm -f mpiVersion openmpVersion hybridVT sortingHoare sortingLomuto *.o
