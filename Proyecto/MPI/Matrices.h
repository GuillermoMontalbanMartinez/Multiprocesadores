#ifndef A42_MATRICES // Esto es un include guard
#define A42_MATRICES // https://en.wikipedia.org/wiki/Include_guard

double** reservarMatriz(int fila, int columna);

void llenarMatrizAleatoria(int fila, int columna, double** matriz, int min, int max);

void liberarMatriz(double** matriz);

void mostrarMatriz(int fila, int collumna, double** matriz);

void aplicarOperaction(int tam, double** matrizA, int(*func)(int, int), 
										double** matrizB, double** matrizC);

int volcarMatriz(const char* nombreArchivo, int tam, double** matriz);

int cargarMatriz(const char* nombreArchivo, int* tam, double*** matriz);

#endif
