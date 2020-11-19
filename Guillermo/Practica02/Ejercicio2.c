
/****************************** EJERCICIO 02 *********************************
**	Desarrollar un programa que busque el mínimo de un vector de valores 	**
**	reales (double) aleatorios de forma paralela. La longitud del vector	**
**	del vector se debe indicar por parámetro. La carga de trabajo debe 		**
**	balancearse de la forma más equilibrada posible. El procesador cero es	**
** 	el que dispone de la información sobre el vector. El procesador dos 	**
** 	será el encargado de recoger la información y mostrarla por pantalla.	**
******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "vectores.h"

int main(int arg, char *argv[]) {
	int size = 0, rank = 0;
	int length = 0;
	double *vector = NULL;
	double mininum = 9999.99;

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
		} while (length <= 0);

		vector = (double*) calloc(length,sizeof(double));
		srand48(time(NULL));

		for (int i = 0; i < length; i++) {
			vector[i] = drand48() * (1000.0 - 1.0) + 1.0; // numRandom = dran48() * (n-m) + n;
		}

	} else if (rank == 1) {

	}


	MPI_Finalize();

	return 0;

}