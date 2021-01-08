#ifndef MATRICES_PROYECTO_FINAL
#define MATRICES_PROYECTO_FINAL
    double** reservarMatriz(int fila, int columna);
    void liberarMatriz(double** matriz);
    void llenarMatrizAleatoria(int fila, int columna, double** matriz, int min, int max);
    void mostrarMatriz(int fila, int columna, double** matriz);
    double buscarMinimoMatriz(int fila, int columna, double** matriz);
    void multiplicarMatrices(double** matriz_c, int filas_matriz_a, int columnas_matriz_b, double** matriz_a, int columnas_matriz_a, double** matriz_b, int filas_matriz_B);
#endif