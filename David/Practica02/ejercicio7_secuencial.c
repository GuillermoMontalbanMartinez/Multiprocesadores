#include<stdio.h>
#include"vectores.h"
#include<time.h>
#include<math.h>

double calcular_coseno(double *a, double *b, int n){

    double cos, cosAux, producto, raizA, raizB, potenciaA, potenciaB;
    cos =cosAux = producto = raizA = raizB = potenciaA = potenciaB =0.0;

    for(int i = 0; i<n; i++){
        
        producto += a[i] * b[i];

        potenciaA += pow(a[i], 2);
        potenciaB += pow(b[i], 2);

        raizA = sqrt(potenciaA);
        raizB = sqrt(potenciaB);        
        
    }
    cos = producto / (raizA*raizB);
    return cos;
}

int main(int argc, char **argv){
    double *vectorA, *vectorB, n;
    
    clock_t t_ini, t_fin;
    double secs;

    t_ini = clock();
    
    srand(time(NULL)); 

    if(argc != 2){
        printf("Lanzar el programa con la estructura : ./ejercicio6 <int>");
    }else{
        n = atoi(argv[1]);
        if(n<0){
            printf("Se han establecido valores negativos para la longuitud de los vectores, lance el programa de nuevo asegurandose de que el valor de longuitud sea positivo y etero");
            return -1;
        }
    }

    vectorA = (double*)malloc(n*sizeof(double));
    vectorB = (double*)malloc(n*sizeof(double));

    fillVector_double(vectorA, n);
    fillVector_double(vectorB, n);

    printf("Vector A : ");
    showVector_double(vectorA, n);

    printf("Vector B : ");
    showVector_double(vectorB, n);

    double cos = calcular_coseno(vectorA, vectorB, n);
    printf("Coseno = %1.5f\n", cos);
    free(vectorA);
    free(vectorB);

    t_fin = clock();

    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecución %.16g milisegundos\n", secs * 1000.0);

    return  0;
}