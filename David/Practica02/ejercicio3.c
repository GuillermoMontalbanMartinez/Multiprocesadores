#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "vectores.h"
#include<math.h>

int main (int argc, char* argv[]) {
    int size = 0, rank = 0;
    int n = 0;
    int n_core = 0;
    int resto = 0;
    int *local_escalar = NULL;
    int *escalar = NULL;
    int *a = NULL;
    int *b = NULL;
    int *local_a = NULL;
    int *local_b = NULL;
    int *tam = NULL;
    int *ini = NULL;
 
    srand(time(NULL)); // Numero enteros aleatorios
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Indica el numero de nucleos disponible del procesador
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // ID del nucleo que estamos usando
    MPI_Status status; // Flag de finalización del proceso
 
    if (size != 4) {
        printf("El programa está pensado para ser ejecutado con 4 nucleos. \n Repita la ejecución utilizando este número de nucleos \n");
        MPI_Finalize();
        exit(0);
    }

    
    if(argc != 2){
        printf("Lance el programa con el siguiente formato : mpirun -np 4 ./ejercicio3 <int>\n");
        MPI_Finalize();
        exit(0);
    }

    tam = (int*)calloc(size,sizeof(int));
    ini = (int*)calloc(size,sizeof(int));

    if (rank == 0) {
        n = atoi(argv[1]);       
        n_core = n / size;
        resto = n % size;

        for(int i = 1; i < size; i++){
            MPI_Send(&n_core, 1, MPI_INT, i, 111, MPI_COMM_WORLD);
            MPI_Send(&resto, 1, MPI_INT, i, 111, MPI_COMM_WORLD);
        }
    }


    if(rank != 0){
        MPI_Recv(&n_core, 1, MPI_INT, 0, 111, MPI_COMM_WORLD, &status);
        MPI_Recv(&resto, 1, MPI_INT, 0, 111, MPI_COMM_WORLD, &status);

    }
    
    int aux = 0;    
    for(int i = 0; i < size; i++){
        ini[i] = aux;
        
        tam[i] = n_core;
        if(resto != 0){
            tam[i]++;
            resto--;
        }
        aux = ini[i] + tam[i];
    }


    if(rank == 0){        
 
        a = (int*) calloc(n, sizeof(int));
        b = (int*) calloc(n, sizeof(int));
        escalar = (int*) calloc(n, sizeof(int));
        fillVector_int(a, n);
        printf("Vector A\n");
        showVector_int(a, n);

        fillVector_int(b, n);
        printf("Vector B\n");
        showVector_int(b, n);

    }

    local_a = (int*)calloc(tam[rank] , sizeof(int));
    local_b = (int*)calloc(tam[rank] , sizeof(int));
    local_escalar = (int*)calloc(tam[rank] , sizeof(int));

    
    MPI_Scatterv(a, tam, ini, MPI_INT, local_a, tam[rank], MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(b, tam, ini, MPI_INT, local_b, tam[rank], MPI_INT, 0, MPI_COMM_WORLD);

    for(int i = 0; i < tam[rank]; i++){
        local_escalar[i] = local_a[i] * local_b[i];
    }
    
    MPI_Gatherv(local_escalar, tam[rank], MPI_INT, escalar, tam, ini, MPI_INT,0, MPI_COMM_WORLD);    
    
    if(rank == 0){
        printf("El producto escalar de los vectores A y B es el siguiente vector\n");
        showVector_int(escalar, n);
        free(escalar);
    }

    MPI_Finalize();
    
    free(local_a);
    free(local_b);
    free(tam);
    free(ini);

    return 0;
}