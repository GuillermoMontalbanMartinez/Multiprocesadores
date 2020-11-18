#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <stdbool.h>
#include "vectores.h"

void fillVector(int*vector, int n) {
	for (int i = 0; i < n; i++) {
		vector[i] = rand() % 100;
	}
}

void showVector(int*vector, int n) {
	printf("[");
	for (int i = 0; i < n; i ++) {
		if (i != n - 1) {
			printf("%i ,", vector[i]);
		} else {
			printf("%i", vector[i]);
		}

	}
	printf("]\n");
}

bool isNumberEven(int number_of_clients, int size) {
	if (number_of_clients % size == 0) {
		return true;
	} else {
		return false;
	}
}


