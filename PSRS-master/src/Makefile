
CC := mpicc

PSRS : mpiVersion.o
	$(CC) -o mpiVersion mpiVersion.o

PSRS.o : PSRS.c
	$(CC) -c mpiVersion.c -o mpiVersion.o

clean :
	-rm -f mpiVersion *.o
