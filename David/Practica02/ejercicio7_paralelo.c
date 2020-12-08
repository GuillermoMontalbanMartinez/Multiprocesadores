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
    double *global_dataA = NULL;
    double *global_dataB = NULL;
    double *local_dataA = NULL;
    double *local_dataB = NULL;
    int *tam = NULL;
    int *ini = NULL;
    double t_ini, t_fin, secs, total_secs;
    double cos, cosAux, producto, raizA, raizB, potenciaA, potenciaB;
    double auxProducto, auxA, auxB;

    cos =cosAux = producto = raizA = raizB = potenciaA = potenciaB =0.0;
    

 
    srand(time(NULL)); // Numero enteros aleatorios
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Indica el numero de nucleos disponible del procesador
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // ID del nucleo que estamos usando
    MPI_Status status; // Flag de finalización del proceso
 
    t_ini = MPI_Wtime();


    if(argc != 2){
        printf("Lance el programa con el siguiente formato : mpirun -np <int> ./ejercicio3 <int>\n");
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
 
        global_dataA = (double*) calloc(n, sizeof(double));
        global_dataB = (double*) calloc(n, sizeof(double));
        fillVector_double(global_dataA, n);
        fillVector_double(global_dataB, n);

        printf("Vector A\n");
        showVector_double(global_dataA, n);

        printf("Vector B\n");
        showVector_double(global_dataB, n);

    }

    local_dataA = (double*)calloc(tam[rank] , sizeof(double));
    local_dataB = (double*)calloc(tam[rank] , sizeof(double));

    
    MPI_Scatterv(global_dataA, tam, ini, MPI_DOUBLE, local_dataA, tam[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(global_dataB, tam, ini, MPI_DOUBLE, local_dataB, tam[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);





    for(int i = 0; i<tam[rank]; i++){
        
        producto += local_dataA[i] * local_dataB[i];

        potenciaA += pow(local_dataA[i], 2);
        potenciaB += pow(local_dataB[i], 2);
       
    }

    MPI_Reduce(&producto, &auxProducto, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&potenciaA, &auxA, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&potenciaB, &auxB, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank==0){            
        raizA = sqrt(auxA);
        raizB = sqrt(auxB);   
        cos = auxProducto / (raizA*raizB);
        printf("El coseno de los vectores es de : %1.6f\n", cos);     
    }


    t_fin = MPI_Wtime();

    secs = (double)( t_fin - t_ini);    
            

    MPI_Reduce(&secs, &total_secs, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    
    if(rank == 0){
        printf("El tiempo de ejecución ha sido %.16f\n", total_secs);
        
        free(global_dataA);
        free(global_dataB);
    }

    MPI_Finalize();
    
    free(local_dataB);
    free(local_dataA);
    free(tam);
    free(ini);

    return 0;
}