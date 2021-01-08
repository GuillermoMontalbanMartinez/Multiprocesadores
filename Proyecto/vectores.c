#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include "vectores.h"


double** reservarMatriz(int fila, int columna){
	double** matriz = 0;
	double* matriz_container = malloc(sizeof(double)*fila*columna);//Gran vector contiguo
	if(matriz_container){
		matriz = malloc(sizeof(double*)*fila);//Aqui guardaremos el comienzo de cada fila
		if(matriz){
			for(int i = 0; i < fila; i++){//Vamos a ir apuntando a cada fila
				matriz[i] = &(matriz_container[i*columna]);//El inicio de la fila i es i*columnas
			}
		}else{//Ahora pediir memoria ha fallado... pero:
			printf("Error. No se ha podido reservar la carcasa 2D de la matriz.\n");
			free(matriz_container);//Esto si funciono, y debemos limpiarlo
		}
	}else{
		printf("Error. No se ha podido reservar memoria para la matriz.\n");
	}
	return matriz;//Tenemos esta memoria heap... y luego otro debera liberarla eh
}

void liberarMatriz(double** matriz){
	if(matriz){//Liberando el principio de la matriz ya estamos apuntando al principio del container
		free(matriz[0]);//Pero claro, solo si apunta a algo
	}
	free(matriz);//Ahora podemos eliminar el conjunto de punteros a fila
}

void llenarMatrizAleatoria(int fila, int columna, double** matriz, int min, int max){
	int diff = max-min+10;
	for(int i = 0; i < fila; i++){
		for(int j = 0; j < columna; j++){
			matriz[i][j] = (drand48()*(double)(diff)) + (double)min - 10.0;
		}
	}
}

void mostrarMatriz(int filas, int columnas, double** matriz){
	for(int i = 0; i<filas; i++){
		for(int j = 0; j<columnas; j++){
			printf("\t%4.4f\t", matriz[i][j]);
		}
		printf("\n");//Terminar fila con salto de linea
	}
}

double buscarMinimoMatriz(int fila, int columna, double** matriz) {
    double minimo = matriz[0][0];

    for (int i = 0; i < fila; i++) {
        for (int j = 0; j < columna; j++) {
            if (minimo > matriz[i][j]) {
                minimo = matriz[i][j];
            }
        }
    }
    
    return minimo;
}

void multiplicarMatrices(double** matriz_c, int filas_matriz_a, int columnas_matriz_b, double** matriz_a, int columnas_matriz_a, double** matriz_b, int filas_matriz_B) {
    // Necesitamos hacer esto por cada columna de la segunda matriz (B)
    for (int a = 0; a < columnas_matriz_b; a++) {
        // Dentro recorremos las filas de la primera (A)
        for (int i = 0; i < filas_matriz_a; i++) {
            double suma = 0.0;
            // Y cada columna de la primera (A)
            for (int j = 0; j < columnas_matriz_a; j++) {
                // Multiplicamos y sumamos resultado
                suma += matriz_a[i][j] * matriz_b[j][a];
            }
            // Lo acomodamos dentro del producto
            matriz_c[i][a] = suma;
        }
    }
}
