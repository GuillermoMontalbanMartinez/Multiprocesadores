
/****************************** EJERCICIO 02 *********************************
**	Desarrollar un programa que busque el mínimo de un vector de valores 	**
**	reales (double) aleatorios de forma paralela. La longitud del vector	**
**	del vector se debe indicar por parámetro. La carga de trabajo debe 		**
**	balancearse de la forma más equilibrada posible. El procesador cero es	**
** 	el que dispone de la información sobre el vector. El procesador dos 	**
** 	será el encargado de recoger la información y mostrarla por pantalla.	**
******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "vectores.h"

int main(int arg, char *argv[]) {
	int size = 0, rank = 0;
	int length_vector = 0;
	double *vector_in = NULL;
	double *vector_out = NULL;
	double mininum = 9999.99;
	double elements_for_the_processor = 0.0;
	int elements_for_the_processor_int = 0;

	srand48(time(NULL));

	MPI_Init(&arg, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size); 
	MPI_Comm_rank(MPI_COMM_WORLD, & rank);
	MPI_Status status;

	if (size != 2) {
		printf("El programa está pensado para ser ejecutado con 2 nucleos. \n Repita la ejecución utilizando este número de nucleos \n");
		MPI_Finalize();
		exit(0);
	}

	if (rank == 0) {
		do {
			printf("Ingrese la longitud del vector: ");
			fflush(stdin);
			fflush(stdout);
			scanf("%i", length);
		} while (length_vector <= 0);

		printf("CPU 0: \n");
		fillVectorDouble(vector_in,length);
		showVectorDouble(vector_in,length);
		printf("\n");

		if (isNumberEven(length)) {
			for (int i = 1; i < size; i++) {
				MPI_Send(&length_vector, 1, MPI_INT, 1, 123, MPI_COMM_WORLD);
			}

			vector_in = (double*) calloc(length_vector,sizeof(double));
			elements_for_the_processor_int = length_vector / size;
			vector_out = (double*) calloc(elements_for_the_processor_int, sizeof(double));

			MPI_Scatter(&vector_in, elements_for_the_processor, MPI_Double, elements_for_the_processor, MPI_Double, 0, MPI_COMM_WORLD);

		} else {
			for (int i = 1; i < size; i++) {
				MPI_Send(&length_vector, 1, MPI_INT, 1, 123, MPI_COMM_WORLD);
			}

			vector_in = (double*) calloc(length_vector,sizeof(double));

			elements_for_the_processor = ((double) length_vector / (double) size);

			if ((elements_for_the_processor / 0.5) == elements_for_the_processor * 2) {
				elements_for_the_processor += 0.1;
			}
		}

	} else if (rank == 1) {
		MPI_Recv(&length_vector, 1, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
		elements_for_the_processor = (double) length / (double) size;

		if () 
		MPI_Scatter(&vector_in,elements_for_the_processor,MPI_Double,elements_for_the_processor,MPI_Double,0,MPI_COMM_WORLD);
	}


	MPI_Finalize();

	return 0;

}
