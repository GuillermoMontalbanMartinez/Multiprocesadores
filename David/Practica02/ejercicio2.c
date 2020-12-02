#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "vectores.h"
#include<math.h>

int main (int argc, char* argv[]) {
    int size = 0, rank = 0;
    int n;
    int n_core = 0;
    int resto = 0;
    int *tam = NULL;
    int *ini = NULL;
    double *global_data;
    double *local_data;
    double min;
    double resultado;
 
    srand(time(NULL)); // Numero enteros aleatorios
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Indica el numero de nucleos disponible del procesador
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // ID del nucleo que estamos usando
    MPI_Status status; // Flag de finalización del proceso
    

    tam = (int*)calloc(size,sizeof(int));
    ini = (int*)calloc(size,sizeof(int));
 
    if (rank == 0) {
 
        do {
            printf("Ingrese el número de clientes de la empresa: ");
            fflush(stdin);
            fflush(stdout);
            scanf("%i",&n);
        } while (n <= 0);
        
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

        global_data = (double*) calloc(n, sizeof(double)); 
        fillVector_double(global_data, n);
        printf("Vector");
        showVector_double(global_data, n);
    }

    local_data = (double*)calloc(tam[rank] , sizeof(double));
    
    MPI_Scatterv(global_data, tam, ini, MPI_DOUBLE, local_data, tam[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

    min = findMin_double(local_data, tam[rank]);
    MPI_Reduce(&min, &resultado, 1, MPI_DOUBLE, MPI_MIN, 2, MPI_COMM_WORLD);

    if(rank == 2){      
        printf("El valor minimo del vector es: %1.1f\n", resultado);          
    }   
    

    if(rank==0){
        free(global_data);
    }
    free(local_data);
    free(tam);
    free(ini);
    MPI_Finalize();
    
    return 0;
}