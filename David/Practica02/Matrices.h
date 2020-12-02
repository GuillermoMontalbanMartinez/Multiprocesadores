#ifndef A42_MATRICES // Esto es un include guard
#define A42_MATRICES // https://en.wikipedia.org/wiki/Include_guard

int** reservarMatriz(int fila, int columna);

void llenarMatrizAleatoria(int fila, int columna, int** matriz, int min, int max);

void liberarMatriz(int** matriz);

void mostrarMatriz(int fila, int collumna, int** matriz);

void aplicarOperaction(int tam, int** matrizA, int(*func)(int, int), 
										int** matrizB, int** matrizC);

int volcarMatriz(const char* nombreArchivo, int tam, int** matriz);

int cargarMatriz(const char* nombreArchivo, int* tam, int*** matriz);

#endif
