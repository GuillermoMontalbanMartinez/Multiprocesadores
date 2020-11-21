#ifndef _VECTORESH
#define _VECTORESH

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void fillVector_int(int *vector, int n);
void showVector_int(int *vector, int n);
void fillVector_double(double *vector, int n);
void showVector_double(double *vector, int n);
double findMin_double(double *vector, int n);

#endif