#include<stdio.h>
#include<stdlib.h>
#include"Matrices.h"
#include<pthread.h>
#include<stdbool.h>
double **final_matrix;

typedef struct Data {
    double **matrixA;
    double**matrixB;
    int filasA;
    int columnasA;
    int filasB;
    int columnasB;
    int ini;
    int fin;
    
}data;

void *multiply_matrix(void *args){
    data *datos = (data*)args;
    for(int j = 0; j < datos->columnasB; j++){
        for(int i = datos->ini; i < datos->fin; i++){
            double suma = 0;
          
            for(int k = 0; k < datos->columnasA; k++){
                suma += datos->matrixA[i][k] * datos->matrixB[k][j];                
            }   
            
            final_matrix[i][j] = suma;
        }
    }  
}




int main(int argc, char **argv){
    srand(time(NULL));
    int columnasA, columnasB, filasA, filasB, n_threads, pack, offset;
    double **matrixA, **matrixB;   
    data *datos;

    if(argc != 2){
        printf("Lance el programa con 2 parámetros ./name <int>");
        return -1;
    }

    n_threads = atoi(argv[1]);

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

    
    final_matrix = reservarMatriz(filasA, columnasB);

    pack = filasA/n_threads;
    offset = filasA % n_threads;

   
 
    pthread_t *threads = malloc(sizeof(pthread_t)*n_threads);

    datos = (data*) malloc(sizeof(data)*n_threads);
    printf("---------------MatrizA-----------------\n");
    matrixA = reservarMatriz(filasA, columnasA);
    llenarMatrizAleatoria(filasA, columnasA, matrixA, -10, 10);
    mostrarMatriz(filasA, columnasA, matrixA);

    printf("---------------MatrizB-----------------\n");
    matrixB = reservarMatriz(filasB, columnasB);
    llenarMatrizAleatoria(filasB, columnasB, matrixB, -10, 10);
    mostrarMatriz(filasB, columnasB, matrixB);

    

    int pos = 0;
    int fin;
    for(int i = 0; i < n_threads; i++){
        datos[i].matrixA = matrixA;
        datos[i].matrixB = matrixB;
        datos[i].columnasB = columnasB;
        datos[i].filasB = filasB;
        datos[i].columnasA = columnasA;
        datos[i].filasA = columnasA;
        datos[i].ini = pos;
        fin = pos + pack;
        if(offset!=0){
            fin++;
            offset--;
        }
        pos = fin;
        datos[i].fin = fin;



                
       pthread_create(&threads[i], 0, multiply_matrix , &datos[i]); 


        for(int i = 0; i<n_threads; i++){
			pthread_join(threads[i], 0);
		}
    }

    printf("---------------MatrizC-----------------\n");

    mostrarMatriz(filasA, columnasB, final_matrix);

    double min = encontrarMinimo(filasA, columnasB, final_matrix);

    printf("\nEl minimo de la matriz resultante del producto de las matrices A y B es: %1.1f\n", min);

    free(datos);
    liberarMatriz(matrixA);
    liberarMatriz(matrixB);
    liberarMatriz(final_matrix);
    free(threads);

}
