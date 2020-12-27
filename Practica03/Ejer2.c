#include<stdio.h>
#include"vectores.h"
#include<pthread.h>
#include<time.h>
#define INFINITY 9999999999999.9


pthread_mutex_t mutex;
double min = INFINITY;

typedef struct Data{
    double *vector;
    int start;
    int end;
}data;

void *find_min(void *arg){
    
    data *datos = (data*) arg;
    for(int i = datos->start; i < datos->end; i++){
        if(datos->vector[i]<min){
            pthread_mutex_lock(&mutex);
            min = datos->vector[i];
            pthread_mutex_unlock(&mutex);
        }
    }
}
int main(int argc, char **argv) {

    srand(time(NULL));
    double *vec;
    int n, n_threads, pack, offset;   
    data *datos;

    if(argc != 2){
        printf("Lance el programa con 2 parámetros ./name <int>");
        return -1;
    }

    n_threads = atoi(argv[1]);

    printf("Introduzca el número de elementos del vector :");
    scanf("%d", &n);
    if(n < n_threads){
        printf("El numero de clientes de la empresa no puede ser menor al número de hilos\n");
        return -1;
    }
   
    pack = n/n_threads;
    offset = n % n_threads;
 
    pthread_t *threads = malloc(sizeof(pthread_t)*n_threads);
    pthread_mutex_init(&mutex, 0);

    datos = (data*) malloc(sizeof(data)*n);
    vec = (double*) malloc(sizeof(double)*n);

    fillVector_double(vec, n);

    printf("Vector: ");
    showVector_double(vec, n);


    for(int i = 0; i < n_threads; i++){
        datos[i].vector = vec;
        datos[i].start = i*pack;
        datos[i].end = i*pack + pack;

        pthread_create(&threads[i], 0, find_min, &datos[i]); 

        for(int i = 0; i<n_threads; i++){
			pthread_join(threads[i], 0);
		}
    }
        printf("\nEl minimo del vector es   : %1.1f\n", min);

    free(vec);
    pthread_mutex_destroy(&mutex);
    free(datos);
    free(threads);
    return 0;
}