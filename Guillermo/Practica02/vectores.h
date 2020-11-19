#ifndef _VECTORES_H_
#define _VECTORES_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <stdbool.h>

void fillVector(int*vector, int n);
void fillVectorDouble(double *vector, int size);
void showVector(int*vector, int n);
void showVectorDouble(double *vector, int size);
bool isNumberEven(int number_of_clients, int size);
bool isNumberEvenV2(int length);
double mininumVectorNumber(double *vector, int size);

#endif
