#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include "vectores.h"

#define MAX_RANGO 10.0
#define MIN_RANGO 0.0

int main(int argc, char* argv[]) {
    double** matriz_a = NULL;
    double** matriz_b = NULL;
    double** matriz_c = NULL;
    int fila_matriz_a = 0;
    int columna_matriz_a = 0;
    int fila_matriz_b = 0;
    int columna_matriz_b = 0;
    double minimo = DBL_MAX;

    srand48(time(NULL));

    do {
        do {
            printf("Introduce el tamaño de la fila de la matriz A: ");
            fflush(stdin);
            fflush(stdout);
            scanf("%d",&fila_matriz_a);
        } while (fila_matriz_a <= 0);

        do {
            printf("Introduce el tamaño de la columna de la matriz A: ");
            fflush(stdin);
            fflush(stdout);
            scanf("%d",&columna_matriz_a);
        } while (columna_matriz_a <= 0);

        do {
            printf("Introduce el tamaño de la fila de la matriz B: ");
            fflush(stdin);
            fflush(stdout);
            scanf("%d",&fila_matriz_b);
        } while (fila_matriz_b <= 0);

        do {
            printf("Introduce el tamaño de la columna de la matriz B: ");
            fflush(stdin);
            fflush(stdout);
            scanf("%d",&columna_matriz_b);
        } while (columna_matriz_b <= 0);

        if (columna_matriz_a != fila_matriz_b) {
            printf("\nERROR: No se puede multiplicar matrices si la columnas de la matriz A es distinta a las filas de la matriz B\n\n");
        }
        
    } while (columna_matriz_a != fila_matriz_b);

    matriz_a = reservarMatriz(fila_matriz_a, columna_matriz_a);
    matriz_b = reservarMatriz(fila_matriz_b, columna_matriz_b);
    matriz_c = reservarMatriz(fila_matriz_a, columna_matriz_b);

    llenarMatrizAleatoria(fila_matriz_a, columna_matriz_a, matriz_a, MIN_RANGO, MAX_RANGO);
    llenarMatrizAleatoria(fila_matriz_b, columna_matriz_b, matriz_b, MIN_RANGO, MAX_RANGO);

    printf("-----------Matriz A-----------\n");
    mostrarMatriz(fila_matriz_a, fila_matriz_b, matriz_a);
    printf("-----------------------------\n\n");

    printf("-----------Matriz B-----------\n");
    mostrarMatriz(fila_matriz_b, columna_matriz_b, matriz_b);
    printf("-----------------------------\n\n");

    multiplicarMatrices(matriz_c, fila_matriz_a, columna_matriz_b, matriz_a, columna_matriz_a, matriz_b, fila_matriz_b);

    printf("-----------Matriz C-----------\n"); 
    mostrarMatriz(fila_matriz_a, fila_matriz_b, matriz_c);
    printf("-----------------------------\n\n");

    minimo = buscarMinimoMatriz(fila_matriz_a, columna_matriz_b, matriz_c);

    printf("\nEL mínimo del vector resultante de la multiplicación es: %4.4f", minimo);

    liberarMatriz(matriz_a);
    liberarMatriz(matriz_b);
    liberarMatriz(matriz_c);


    return 0;
}