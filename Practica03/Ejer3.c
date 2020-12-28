#include<stdio.h>
#include"Matrices.h"
#include<pthread.h>
#include<time.h>
#include<stdlib.h>

pthread_mutex_t mutex;
int sum=0;
typedef struct Data{
    int **matrix;
    int n_elements;
    int start;
    int end;
}data;

void *sum_matrix_filas(void *arg){
    data *datos = (data*)arg;  
    
    for(int i = datos->start; i < datos->end; i++){
        for(int j = 0; j < datos->n_elements; j++){
            pthread_mutex_lock(&mutex);
            sum+= datos->matrix[i][j];
            pthread_mutex_unlock(&mutex);
        }       
        
    }
    
}

void *sum_matrix_columnas(void *arg){
    data *datos = (data*)arg;  
    
    for(int j = 0; j < datos->n_elements; j++){
        for(int i = datos->start; i < datos->end; i++){
            pthread_mutex_lock(&mutex);
            sum+= datos->matrix[i][j];
            pthread_mutex_unlock(&mutex);
        }       
        
    }
    
}
int main(int argc, char **argv) {

    srand(time(NULL));
    int **matrix;
    int opcion, row, col, n_threads, pack, offset;   
    data *datos;

    if(argc != 2){
        printf("Lance el programa con 2 parámetros ./name <int>");
        return -1;
    }

    n_threads = atoi(argv[1]);


    printf("Introduzca el número de filas de la matriz :");
    scanf("%d", &row);
    
    printf("Introduzca el número de columnas de la matriz :");
    scanf("%d", &col);
   

    printf("Desea realizar el reparto de la matriz por filas(0) o por columnas(1)\n Introduzca el codigo asociado a cada opcion :");
    scanf("%d", &opcion);

    datos = (data*) malloc(sizeof(data)*n_threads);
    matrix = reservarMatriz(row, col);
    llenarMatrizAleatoria(row, col, matrix, 0, 1);

    pack = row/n_threads;
    offset = row % n_threads;
 
    pthread_t *threads = malloc(sizeof(pthread_t)*n_threads);
    pthread_mutex_init(&mutex, 0);

   
    int pos=0;
    for(int i = 0; i < n_threads; i++){
        datos[i].matrix = matrix;
        datos[i].start = pos;
        datos[i].n_elements = col;
        if(offset!=0){
            pos+=pack+1;
            offset--;
        } else{
            pos += pack;
        }
        datos[i].end = pos;

        if(opcion==0){
            pthread_create(&threads[i], 0, sum_matrix_filas, &datos[i]); 
        }
        if(opcion==1){
            pthread_create(&threads[i], 0, sum_matrix_columnas, &datos[i]); 
        }


        for(int i = 0; i<n_threads; i++){
			pthread_join(threads[i], 0);
		}
    }
    printf("Matriz: \n");
    mostrarMatriz(row, col, matrix);
    printf("\nLa suma de elementos del vector es: %d\n", sum);

    liberarMatriz(matrix);
    pthread_mutex_destroy(&mutex);
    free(datos);
    free(threads);
    return 0;
}