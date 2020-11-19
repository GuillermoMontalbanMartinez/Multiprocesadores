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

void fillVectorDouble(double *vector, int size) {
	for (int i = 0; i < size; i++) {
		vector[i] = drand48() * (1000.0 - 1.0) + 1.0; 
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

void showVectorDouble(double *vector, int size) {
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

bool isNumberEvenV2(int length) {
	if (length % 2 == 0) {
		return true;
	} else {
		return false;
	}
}

double mininumVectorNumber(double *vector, int size) {
	for (int i = 0; i < n; i++) {
		int mininum = 0;
		if (i == 0) {
			mininum = vector[i];
		} else {
			if (mininum > vector[i]) {
				mininum = vector[i];
			}
		}
	}
	return mininum;
}

