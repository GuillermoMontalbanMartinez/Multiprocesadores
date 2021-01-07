#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Matrices.h"

srand(time(NULL));

void rellenarMatrizAleatoriaDoubles(double** matriz, int row, int column) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			matriz[i][j] = drand48() * (20-10) - 10;
		}
	}
}

void mostrarMatriz(double** matriz, int row, int column) {
	printf("------------Imprimiendo matriz------------\n");

	for (int i = 0; i < row; i++) {
		for (int j= 0; j < column; j++) {
			printf("%f ",matriz[i][j]);
		}
		printf("\n");
	}
	printf("\n------------------------------------------\n");
}	

double** reservarMatriz(int row, int column) {
	double** matriz = NULL;
	double* matriz_container = malloc(sizeof(double)*row*column);
	if (matriz_container) {
		matriz = malloc(sizeof(int*)*row);
		if (matriz) {
			for (int i = 0; i < row; i++) {
				matriz[i] = &(matriz_container[i*column]);
			}
		} else {
			printf("ERROR: No se ha podido reservar la carcasa 2D de la matriz.\n");
			free(matriz_container);
		}
	} else {
		printf("ERROR: No se ha podido reservar memoria para la matriz.\n");
	}

	/*
	return matriz;
	matriz = (double **)malloc(row,sizeof(double*));

	for (uint32_t i = 0; i < row; i++) {
		matriz[i] = (double *) malloc(column*sizeof(int));
	}

	return matriz;
	*/
}