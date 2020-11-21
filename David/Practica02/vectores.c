#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "vectores.h"

void fillVector_int(int *vector, int n) {
    for (int i = 0; i < n; i++) {
        vector[i] = rand() % 100;
    }
}

void showVector_int(int *vector, int n) {
    for (int i = 0; i < n; i ++) {
        printf("%i ", vector[i]);
    }
    printf("\n");
}

void fillVector_double(double *vector, int n){
    for(int i=0; i<n; i++){
        vector[i] = (double) (rand()%1000)/10;
    }
}

void showVector_double(double *vector, int n) {
    for(int i=0; i<n; i++){
        printf(" %1.1f ", vector[i]);
    }
    printf("\n");
}

double findMin_double(double *vector, int n){

    double minimo;
    for (int i = 0; i < n; i++) {
        if (i == 0) {
            minimo = vector[i];
        } else {
            if (minimo > vector[i]) {
                minimo = vector[i];
            }
        }
        
    }
    return minimo;
}

