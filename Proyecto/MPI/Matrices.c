#include "Matrices.h"

#include <stdio.h> // Intentamos que cada .c sea lo mas autocontenido
#include <stdlib.h> //posible... dejando los .h tambien lo mas ligeros
					//aunque tambien autonomos para todo tipo que definan/usen

double** reservarMatriz(int fila, int columna){
	double** matriz = 0;
	double* matriz_container = malloc(sizeof(double)*fila*columna);//Gran vector contiguo
	if(matriz_container){
		matriz = malloc(sizeof(double*)*fila);//Aqui guardaremos el comienzo de cada fila
		if(matriz){
			for(int i = 0; i<fila; i++){//Vamos a ir apuntando a cada fila
				matriz[i] = &(matriz_container[i*columna]);//El inicio de la fila i es i*columnas
			}
		}else{//Ahora pediir memoria ha fallado... pero:
			printf("Error. No se ha podido reservar la carcasa 2D de la matriz.\n");
			free(matriz_container);//Esto si funciono, y debemos limpiarlo
		}
	}else{
		printf("Error. No se ha podido reservar memoria para la matriz.\n");
	}
	return matriz;//Tenemos esta memoria heap... y luego otro debera liberarla eh
}

void llenarMatrizAleatoria(int filas, int columnas, double** matriz, int min, int max){
	max *=10;
	min*=10;
	int diff = max-min+1;
	for(int i = 0; i<filas; i++){
		for(int j = 0; j<columnas; j++){
			matriz[i][j] = (double)(rand()%(max-min+1)+ min)/10;
		}
	}
}

void liberarMatriz(double** matriz){
	if(matriz){//Liberando el principio de la matriz ya estamos apuntando al principio del container
		free(matriz[0]);//Pero claro, solo si apunta a algo
	}
	free(matriz);//Ahora podemos eliminar el conjunto de punteros a fila
}

void mostrarMatriz(int fila, int columna, double** matriz){
	for(int i = 0; i<fila; i++){
		for(int j = 0; j<columna; j++){
			printf("%1.1f ", matriz[i][j]);
		}
		printf("\n");//Terminar fila con salto de linea
	}
}

void aplicarOperaction(int tam, double** matrizA, int(*func)(int, int), double** matrizB, double** matrizC){
	for(int i = 0; i<tam; i++){
		for(int j = 0; j<tam; j++){//Aplicamos el operador dado sobre cada elemento (por filas)
			matrizC[i][j] = func(matrizA[i][j], matrizB[i][j]);
		}
	}
}

int volcarMatriz(const char* nombreArchivo, int tam, double** matriz){
	int salida = 1, check = 0;
	FILE* archivoSalida = fopen(nombreArchivo, "w");
	if(archivoSalida){
		check = fprintf(archivoSalida, "%d\n", tam);
		if(!check){
			salida = 0;
			printf("Error. No se ha podido escribir el tamaño de la matriz\n");
		}
		if(salida){
			for(int i = 0; salida==1 && i<tam; i++){
				for(int j = 0; j<tam; j++){
					check = fprintf(archivoSalida, "%1.1f ", matriz[i][j]);
					if(!check){
						salida = 0;
						printf("Error. No se ha podido escribir un elemento de la matriz");
						break;
					}
				}
				check = fprintf(archivoSalida, "\n");
				if(!check){
					salida = 0;
					printf("Error. No se ha podido escribir un salto de linea de la matriz");
				}
			}
		}
		fclose(archivoSalida);
	}else{
		salida = 0;
		printf("Error. No se ha podido abrir el archivo para escribir\n");
	}
	return salida;
}

int cargarMatriz(const char* nombreArchivo, int* tam, double*** matriz){
	int salida = 1, check = 0;
	FILE* archivoEntrada = fopen(nombreArchivo, "r");
	if(archivoEntrada){
		int buffer = 0;
		check = fscanf(archivoEntrada, "%d\n", &buffer);
		if(check==1){
			*tam = buffer;//Modificamos el valor en la funcion que llamo (paso por referencia)
		}else{
			salida = 0;
			printf("Error. No se ha encontrado el tamaño de la matriz\n");
		}
		if(salida){
			(*matriz) = reservarMatriz(buffer, buffer);//Ey, vamos a escribir tambien "fuera" (referencia)
			if(*matriz){
				int miTam = buffer;
				for(int i = 0; salida==1 && i<miTam; i++){
					for(int j = 0; j<miTam; j++){
						check = fscanf(archivoEntrada, "%d ", &buffer);
						if(check==1){
							(*matriz)[i][j] = buffer;//Escribiendo fuera... (referencia)
						}else{
							salida = 0;
							liberarMatriz(*matriz);
							printf("Error. No se ha podido leer el elemento de la matriz\n");
							break;
						}
					}
				}
			}
		}
		fclose(archivoEntrada);
	}else{
		salida = 0;
		printf("Error. No se ha podido abrir el archivo para leer\n");
	}
	if(!salida){
		*matriz = 0;//Somos amables y la dejamos apuntada a NULL por seguridad
	}
	return salida;
}
