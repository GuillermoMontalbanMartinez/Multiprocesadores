#include<stdio.h>
#include"Matrices.h"
#include<pthread.h>
#include<time.h>
#include<stdlib.h>

pthread_mutex_t mutex;
int sum=0;
typedef struct Data{
    int **matrix;
    int row;
    int start;
    int end;
}data;

void *sum_matrix(void *arg){
    data *datos = (data*)arg;  
    
    for(int i = datos->start; i < datos->end; i++){
        for(int j = 0; j < datos->row; j++){
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
    llenarMatrizAleatoria(row, col, matrix, 0, 9);

    if(opcion == 0){
        int**aux;
        aux = reservarMatriz(col, row);
        for(int i = 0; i < row; i++){
            for (int j = 0; j < col; j++){
                aux[j][i] = matrix[i][j];
            }
        }

        liberarMatriz(matrix);
        matrix = reservarMatriz(col, row);
        for(int i = 0; i < row; i++){
            for (int j = 0; j < col; j++){
                matrix[i][j] = aux[i][j];
            }
        }
        liberarMatriz(aux);
        
    }

    pack = col/n_threads;
    offset = col % n_threads;
 
    pthread_t *threads = malloc(sizeof(pthread_t)*n_threads);
    pthread_mutex_init(&mutex, 0);

   

    for(int i = 0; i < n_threads; i++){
        datos[i].matrix = matrix;
        datos[i].start = i*pack;
        datos[i].end = i*pack + pack;
        datos[i].row = col;
        if(offset!=0){
            datos[i].row=row+1;
            offset--;
        }

        pthread_create(&threads[i], 0, sum_matrix, &datos[i]); 

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