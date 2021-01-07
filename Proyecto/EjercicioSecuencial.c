#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Matrices.h"

int main(int argc, char *argv[]) {
	double *matriz_a = NULL;
	double *matriz_b = NULL;
	double *matriz_c = NULL;
	int row_matriz_a = 0;
	int column_matriz_a = 0;
	int row_matriz_b = 0;
	int column_matriz_b = 0;

	// srand48(time(NULL));

	do {
			do {
			printf("Introduce el tamaño de las filas de la matriz A: ");
			fflush(stdin);
			fflush(stdout);
			scanf("%u", &row_matriz_a);
		} while (row_matriz_a <= 0);

		do {
			printf("\n Introduce el tamaño de las columnas de la matriz A");
			fflush(stdin);
			fflush(stdout);
			scanf("%u", &column_matriz_a);
		} while(column_matriz_a <= 0);

		do {
			printf("Introduce el tamaño de las filas de la matriz B: ");
			fflush(stdin);
			fflush(stdout);
			scanf("%u", &row_matriz_b);
		} while (row_matriz_b <= 0);

		do {
			printf("Introduce el tamaño de las columnas de la matriz B: ");
			fflush(stdin);
			fflush(stdout);
			scanf("%u", &row_matriz_b);
		} while (column_matriz_b <= 0);
	} while (row_matriz_a != column_matriz_b);


	&matriz_a = reservarMatriz(row_matriz_a, column_matriz_a);
	rellenarMatriz(&matriz_a, row_matriz_a, column_matriz_a);

	mostrarMatriz(&matriz_a, row_matriz_a, column_matriz_a); 


	free(matriz_a);
	free(matriz_b);
	free(matriz_c);

	return 0;
}
