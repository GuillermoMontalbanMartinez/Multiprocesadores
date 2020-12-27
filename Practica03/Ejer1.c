#include<stdio.h>
#include"vectores.h"
#include<pthread.h>
#include<time.h>

pthread_mutex_t mutex_sum, mutex_sub;
int sum_ingresos, sum_gastos;

typedef struct Data{
    int *vector;
    int start;
    int end;
}data;

void* pararel_sum_ingresos(void* arg){
    data *vec_ingresos = (data*) arg;

    for(int i = vec_ingresos->start; i < vec_ingresos->end; i++){
        
        pthread_mutex_lock(&mutex_sum);
        sum_ingresos+=(vec_ingresos->vector)[i];
        pthread_mutex_unlock(&mutex_sum);
    }

}
void* pararel_sum_gastos(void* arg){
    data *vec_ingresos = (data*) arg;

    for(int i = vec_ingresos->start; i < vec_ingresos->end; i++){
        
        pthread_mutex_lock(&mutex_sum);
        sum_gastos+=(vec_ingresos->vector)[i];
        pthread_mutex_unlock(&mutex_sum);
    }

}

int main(int argc, char** argv){
    sum_ingresos = sum_gastos = 0; 
    srand(time(NULL));
    int n, n_threads, pack, offset;    
    int *gastos, *ingresos;
    data *vec_ingresos, *vec_gastos;

    if(argc != 2){
        printf("Lance el programa con 2 argumentos : ./Ejer1 <int>\n");
        return -1;
    }

    n_threads = atoi(argv[1]);

    printf("Ingrese el número de clientes de su empresa: ");
    scanf("%d", &n);

    if(n < n_threads){
        printf("El numero de clientes de la empresa no puede ser menor al número de hilos\n");
        return -1;
    }
    vec_ingresos = (data*) malloc(sizeof(data)*n_threads);
    vec_gastos = (data*) malloc(sizeof(data)*n_threads);

    pthread_t *threads = malloc(sizeof(pthread_t)*n_threads);
    pthread_mutex_init(&mutex_sum, 0);
    pthread_mutex_init(&mutex_sub, 0);
    
    pack = n/n_threads;
    offset = n % n_threads;

    ingresos = (int*) malloc(sizeof(int) * n);
    fillVector_int(ingresos, n);
    printf("\nVector de ingresos: ");
    showVector_int(ingresos, n);

    gastos = (int*) malloc(sizeof(int) * n);
    fillVector_int(gastos, n);
    printf("Vector de gastos: ");
    showVector_int(gastos, n);

    for(int i = 0; i < n_threads; i++){
        vec_ingresos[i].vector = ingresos;
        vec_ingresos[i].start = i*pack;
        vec_ingresos[i].end = i*pack + pack;


        vec_gastos[i].vector = gastos;
        vec_gastos[i].start = i*pack;
        vec_gastos[i].end = i*pack + pack;

        pthread_create(&threads[i], 0, pararel_sum_ingresos, &vec_ingresos[i]); 
        pthread_create(&threads[i], 0, pararel_sum_gastos, &vec_gastos[i]); 


        for(int i = 0; i<n_threads; i++){
			pthread_join(threads[i], 0);
		}
    }
        printf("\nSuma paralelo ingresos: %d\n", sum_ingresos);		
        printf("Suma Paralelo gastos: %d\n", sum_gastos);	

        printf("La resta de ambos es : %d\n", abs(sum_ingresos-sum_gastos));	

		free(threads);
		free(vec_ingresos);
        free(vec_gastos);
		free(ingresos);
        free(gastos);
		pthread_mutex_destroy(&mutex_sum);
        pthread_mutex_destroy(&mutex_sub);
    return 0;
}