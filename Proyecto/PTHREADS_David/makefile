MPI = mpicc
PTHREAD = -lpthread
GCC = gcc
LIBS = -lm
RM      = rm -f


proyecto_mpi:
	$(MPI) vectores.c Matrices.c proyecto_mpi.c -o proyecto_mpi $(LIBS)

proyecto_pthread:
	$(GCC) Matrices.c proyecto_pthread.c -o proyecto_pthread $(PTHREAD)

clean:
	$(RM) proyecto_mpi
	$(RM) proyecto_pthread