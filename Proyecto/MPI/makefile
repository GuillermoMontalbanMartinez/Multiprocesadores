MPI = mpicc
LIBS = -lm
RM      = rm -f


proyecto_mpi:
	$(MPI) vectores.c Matrices.c proyecto_mpi.c -o proyecto_mpi $(LIBS)

clean:
	$(RM) proyecto_mpi