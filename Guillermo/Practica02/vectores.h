#ifndef _VECTORES_H_
#define _VECTORES_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <stdbool.h>

void fillVector(int*vector, int n);
void fillVectorDouble(double *vector, int n);
void showVector(int*vector, int n);
void showVector(double *vector, int n);
bool isNumberEven(int number_of_clients, int size);

#endif