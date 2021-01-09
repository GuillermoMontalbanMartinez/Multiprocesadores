#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "Matrices.h"
#include "vectores.h"
#include<math.h>

int main (int argc, char* argv[]) {
    int size = 0, rank = 0;
    int columnasA = 0;
    int filasA = 0;
    int n_core = 0;
    int resto = 0;
    double **matrizA = NULL;
    double **matrizB = NULL;
    int *tam = NULL;
    int *ini = NULL;
    double *local_data;
    double *vector_auxA;
    double *vector_auxB;
    double *local_vecB;
    double **local_matrixA;
    double **local_matrixB;
    double **producto;
    double suma = 0;
    int filasB =0;
    int columnasB = 0;
    int filas_aux=0;
    double min;
    double resultado;
    int pos;
    
    srand(time(NULL)); // Numero enteros aleatorios
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Indica el numero de nucleos disponible del procesador
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // ID del nucleo que estamos usando
    MPI_Status status; // Flag de finalización del proceso
    

    tam = (int*)calloc(size,sizeof(int));
    ini = (int*)calloc(size,sizeof(int));
 
    if (rank == 0) {
        do{
            printf("Bienvenido, a continuación se le solicitarán las dimensiones para las matrices a multilicar, recuerde que el número de columnas de A deberá ser igual al de filas de B\n");
            do {
                printf("Ingrese el número de filas y columnas de la matrizA (int int): ");
                fflush(stdin);
                fflush(stdout);
                scanf("%d %d",&filasA, &columnasA);

            } while (filasA <= 0 || columnasA <= 0);

            
            do {
                printf("Ingrese el número de filas y columnas de la matrizB (int int): ");
                fflush(stdin);
                fflush(stdout);
                scanf("%d %d",&filasB, &columnasB);

            } while (filasB <= 0 || columnasB <= 0);     

        }while(columnasA!=filasB);

        for(int i = 1; i < size; i++){
            MPI_Send(&filasA, 1, MPI_INT, i, 111, MPI_COMM_WORLD);
            MPI_Send(&columnasA, 1, MPI_INT, i, 111, MPI_COMM_WORLD);

            MPI_Send(&filasB, 1, MPI_INT, i, 111, MPI_COMM_WORLD);
            MPI_Send(&columnasB, 1, MPI_INT, i, 111, MPI_COMM_WORLD);

        }
    }


    if(rank != 0){
        MPI_Recv(&filasA, 1, MPI_INT, 0, 111, MPI_COMM_WORLD, &status);
        MPI_Recv(&columnasA, 1, MPI_INT, 0, 111, MPI_COMM_WORLD, &status);

        MPI_Recv(&filasB, 1, MPI_INT, 0, 111, MPI_COMM_WORLD, &status);
        MPI_Recv(&columnasB, 1, MPI_INT, 0, 111, MPI_COMM_WORLD, &status);

    }

    n_core = filasA / size;
    resto = filasA % size;

    
    pos = 0;    
    for(int i = 0; i < size; i++){
        ini[i] = pos;        
        tam[i] = n_core;
        if(resto != 0){
            tam[i]++;
            resto--;
        }
        tam[i]*= columnasA;
        pos = ini[i] + tam[i];
    }


    if(rank == 0){        
        
        matrizA = reservarMatriz(filasA, columnasA);
        llenarMatrizAleatoria(filasA, columnasA, matrizA, -10, 10);
        printf("\nMatrizA: \n");
        mostrarMatriz(filasA, columnasA, matrizA);
        printf("\n");

        matrizB = reservarMatriz(filasB, columnasB);
        llenarMatrizAleatoria(filasB, columnasB, matrizB, -10, 10);
        printf("MatrizB: \n");
        mostrarMatriz(filasB, columnasB, matrizB);
        printf("\n");

        pos=0;
        vector_auxA = (double*)calloc(filasA * columnasA, sizeof(double));
        for(int i = 0; i < filasA; i++){
            for(int j = 0; j < columnasA; j++){
                vector_auxA[pos++] = matrizA[i][j];
            }
        }                   


        pos=0;
        
        vector_auxB = (double*)calloc((filasB * columnasB), sizeof(double));
        for(int i = 0; i < filasB; i++){
            for(int j = 0; j < columnasB; j++){
                vector_auxB[pos++] = matrizB[i][j];
            }
        }

        for(int i = 1; i < size; i++){
            MPI_Send(vector_auxB,filasB*columnasB , MPI_DOUBLE, i, 010, MPI_COMM_WORLD);
        }        
    }
    
    local_data = (double*)calloc(tam[rank], sizeof(double));

    if(rank!=0){

        int n;
        MPI_Probe(0, 010, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_DOUBLE, &n);

       vector_auxB = (double*)calloc(n ,sizeof(double));

        MPI_Recv(vector_auxB, n, MPI_DOUBLE, 0, 010, MPI_COMM_WORLD, &status);
    }

    MPI_Scatterv(vector_auxA, tam, ini, MPI_DOUBLE, local_data, tam[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD); 

    filas_aux = tam[rank] / columnasA ;

    producto = reservarMatriz(filasA, columnasB);
    
    local_matrixA = reservarMatriz(filas_aux, columnasA);
    int aux=0;
    for(int i = 0; i < filas_aux; i++){
        for(int j = 0; j < columnasA; j++){
            local_matrixA[i][j] = local_data[aux];
            aux++;
        }
    }
   
    aux = 0;          

    
    local_matrixB = reservarMatriz(filasB, columnasB);
    
    for(int i = 0; i < filasB; i++){
        for(int j = 0; j < columnasB; j++){
            local_matrixB[i][j] = vector_auxB[aux];
            aux++;
        }
    }

    int index = 0;

    for(int i = 0; i < filas_aux; i++){
        for(int j = 0; j < columnasB; j++){
            double suma = 0;
          
            for(int k = 0; k < columnasA; k++){
                suma+= local_matrixA[i][k] * local_matrixB[k][j];
                
            }
            
            producto[j][i] = suma;
        }
    }  

    
    pos=0;
    double *vec_producto = (double*)calloc(filasA*columnasB, sizeof(double));
    for(int i = 0; i < filasA; i++){
        for(int j = 0; j < columnasB; j++){
            vec_producto[pos++] = producto[i][j];

        }
    }

    
    if(!(local_matrixA[0][0]< -11 || local_matrixA[0][0]> 11)){
        
        min = (double)findMin_double(vec_producto, columnasB);
    } else{
        min = 99999.0;
    }

    MPI_Reduce(&min, &resultado, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);


    
    if(rank==0){
        printf("\nMinimo: %1.1f \n", resultado);          

        free(vector_auxA);
        liberarMatriz(matrizA);
        liberarMatriz(matrizB);
    }
    free(local_data);
    free(ini);
    free(tam);
    free(vector_auxB);
    free(vec_producto);
     for(int i = 0; i < filas_aux; i++){
       free(local_matrixA[i]);                
    }
    free(local_matrixA);   
    liberarMatriz(local_matrixB);
    for(int i = 0; i < filas_aux; i++){
       free(producto[i]);                
    }
    free(producto); 
    
    MPI_Finalize();
    
    return 0;
}
