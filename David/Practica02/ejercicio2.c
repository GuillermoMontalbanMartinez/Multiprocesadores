#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "vectores.h"
#include<math.h>

int main(int argc, char **argv) {
    int n, rank, size;
    double *vector_entrada, *vector_salida, elementos;
    srand(time(NULL));

    MPI_Init(&argc, &argv);
    MPI_Comm_size( MPI_COMM_WORLD, &size );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Status status;
    

    if(rank==0){
        do {
            printf("Introduce el tamaño del vector: ");
            fflush(stdout);
            scanf("%d", &n);
            
        } while (n <= 0);

        for(int i = 1; i <size; i++){
            MPI_Send(&n, 1, MPI_INT, i, 123, MPI_COMM_WORLD);
        }
        
        vector_entrada = (double*) calloc(n, sizeof(double));
        printf("-%d-", rank);
        
        elementos = (double)n / (double)size;
        if((elementos/0.5)==elementos*2) elementos += 0.1;  

        vector_salida = (double*) calloc((int)round(elementos), sizeof(double));


        fillVector_double(vector_entrada, n);
        showVector_double(vector_entrada, n);


        //Mejorar el factor de % para , si se puede, repartir mejor la carga reduciendo ny repartir los 0 entre varios nucleos

        MPI_Scatter(vector_entrada, (int)round(elementos), MPI_DOUBLE, vector_salida, (int)round(elementos), MPI_DOUBLE, 0, MPI_COMM_WORLD);
        printf("\n");
        showVector_double(vector_salida, (int)round(elementos));

        free(vector_entrada);
        free(vector_salida);

    }else {
        
        printf("-%d-", rank);
        MPI_Recv(&n, 1, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
        elementos = (double)n / (double)size;
        if((elementos/0.5)==elementos*2) elementos += 0.1; 
        
        vector_salida = (double*) calloc((int)round(elementos), sizeof(double));

        MPI_Scatter(vector_entrada, (int)round(elementos), MPI_DOUBLE, vector_salida, (int)round(elementos), MPI_DOUBLE, 0, MPI_COMM_WORLD);

        showVector_double(vector_salida, (int)round(elementos));
        free(vector_salida);
    }
    MPI_Finalize();
    return 0;
}