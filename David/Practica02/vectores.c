#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "vectores.h"

void fillVector(int *vector, int n) {
    for (int i = 0; i < n; i++) {
        vector[i] = rand() % 100;
    }
}

void showVector(int *vector, int n) {
    for (int i = 0; i < n; i ++) {
        printf("%i ", vector[i]);
    }
    printf("\n");
}