#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "Matrices.h"
#include "vectores.h"
#include<math.h>

int main (int argc, char* argv[]) {
    int size = 0, rank = 0;
    int columnas = 0;
    int filas = 0;
    int n_core = 0;
    int resto = 0;
    int **matriz = NULL;
    int *tam = NULL;
    int *ini = NULL;
    int *local_data;
    int *vector_aux;
    int suma = 0;
    int suma_total = 0;
    int opcion;
    
    srand(time(NULL)); // Numero enteros aleatorios
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Indica el numero de nucleos disponible del procesador
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // ID del nucleo que estamos usando
    MPI_Status status; // Flag de finalización del proceso
    

    tam = (int*)calloc(size,sizeof(int));
    ini = (int*)calloc(size,sizeof(int));
 
    if (rank == 0) {
 
        do {
            printf("Ingrese el número de filas y columnas de la matriz: ");
            fflush(stdin);
            fflush(stdout);
            scanf("%d %d",&filas, &columnas);

        } while (filas <= 0 || columnas <= 0);

        printf("Seleccione la división para la suma de los valores de la matriz: \n 1) Por filas \n 2) Por columnas \n");
        fflush(stdin);
        fflush(stdout);
        scanf("%d",&opcion);
              

        for(int i = 1; i < size; i++){
            MPI_Send(&filas, 1, MPI_INT, i, 111, MPI_COMM_WORLD);
            MPI_Send(&columnas, 1, MPI_INT, i, 111, MPI_COMM_WORLD);
        }
    }


    if(rank != 0){
        MPI_Recv(&filas, 1, MPI_INT, 0, 111, MPI_COMM_WORLD, &status);
        MPI_Recv(&columnas, 1, MPI_INT, 0, 111, MPI_COMM_WORLD, &status);

    }

    n_core = filas / size;
    resto = filas % size;

    
    int pos = 0;    
    for(int i = 0; i < size; i++){
        ini[i] = pos;        
        tam[i] = n_core;
        if(resto != 0){
            tam[i]++;
            resto--;
        }
        tam[i]*= columnas;
        pos = ini[i] + tam[i];
    }


    if(rank == 0){        
        int pos=0;
        matriz = reservarMatriz(filas, columnas);
        llenarMatrizAleatoria(filas, columnas, matriz, 1, 100);
        mostrarMatriz(filas, columnas, matriz);
        printf("\n");
        
        vector_aux = calloc(filas * columnas, sizeof(int));
        if(opcion==1){
            for(int i = 0; i < filas; i++){
                for(int j = 0; j < columnas; j++){
                    vector_aux[pos++] = matriz[i][j];
                    printf("%d ", matriz[i][j]);
                }
                printf("\n");

            }

            printf("DIVISION POR FILAS\n");
            showVector_int(vector_aux, columnas*filas);

        }else{
            for(int i = 0; i < columnas; i++){
                for(int j = 0; j < filas; j++){
                    vector_aux[pos++] = matriz[j][i];
                    printf("%d ", matriz[j][i]);
                }
                printf("\n");
                
            }
            printf("DIVISION POR COLUMNAS\n");
        }    
    }
    
    local_data = calloc(tam[rank], sizeof(int));

    MPI_Scatterv(vector_aux, tam, ini, MPI_INT, local_data, tam[rank], MPI_INT, 0, MPI_COMM_WORLD);   

    for(int i = 0; i < tam[rank]; i++){
        suma += local_data[i];
    }
    
    MPI_Reduce(&suma, &suma_total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

     
    if(rank==0){
        
        printf("La suma total de los elementos de la matriz es: %d\n", suma_total);          

        free(vector_aux);
        liberarMatriz(matriz);
    }
    free(local_data);
    free(ini);
    free(tam);

    MPI_Finalize();
    
    return 0;
}
