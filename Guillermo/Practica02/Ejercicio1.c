/*
**************************************Ejercicio 1*********************************************
** 	Una empresa tiene un potencial de N clientes. Para llevar sus cuentas guarda en un 	    **
** 	vector de tamaño N el número de ingresos realizados por sus clientes, mientras que 	    **
** 	en otro guarda el número de gastos de estos. Considere que los datos almacenados		** 
** 	en los vectores son números aleatorios de tipo entero.								    **
** 	Realice un programa utilizando MPI que calcule la suma de todos los ingresos, la 		**
** 	suma de todos los gastos y reste las cantidades. para el diseño del programa tenga      **
** 	en cuenta que el procesador central (aquel con identificador cero) es el que conoce     **
** 	los datos. Él será el encargado de distribuir la carga y de mostrar el resultado final. **
**********************************************************************************************
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <stdbool.h>
#include "vectores.h"


int main (int argc, char* argv[]) {
	int size = 0, rank = 0;
	int numero_clientes = 0;
	int *ingresos = NULL;
	int *gastos = NULL;
	int total_ingresos = 0;
	int total_gastos = 0;
	int diferencia_gastos_ingresos = 0;

	srand(time(NULL)); // Numero enteros aleatorios

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size); // Indica el numero de nucleos disponible del procesador
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // ID del nucleo que estamos usando
	MPI_Status status; // Flag de finalización del proceso

	if (size != 2) {
		printf("El programa está pensado para ser ejecutado con 2 nucleos. \n Repita la ejecución utilizando este número de nucleos \n");
		MPI_Finalize();
		exit(0);
	}

	if (rank == 0) {

		do {
			printf("Ingrese el número de clientes de la empresa: ");
			fflush(stdin);
			fflush(stdout);
			scanf("%i",&numero_clientes);
		} while (numero_clientes <= 0);

		MPI_Send(&numero_clientes,1,MPI_INT,1,123,MPI_COMM_WORLD);

		ingresos = (int*) calloc(numero_clientes, sizeof(int));
		gastos = (int*) calloc(numero_clientes, sizeof(int));

		fillVector(ingresos,numero_clientes);
		fillVector(gastos,numero_clientes);
		printf("\nVector ingresos\n");
		showVector(ingresos,numero_clientes);
		printf("Vector gastos\n");
		showVector(gastos,numero_clientes);

		// Enviamos los fragmentos de los vectores al core 1
		if (isNumberEven(numero_clientes, size)) {
			MPI_Send(&ingresos[numero_clientes / size], (numero_clientes / size), MPI_INT, 1, 123, MPI_COMM_WORLD);
			MPI_Send(&gastos[numero_clientes / size], (numero_clientes / size), MPI_INT, 1, 123, MPI_COMM_WORLD);

			for (int i = 0; i < ((numero_clientes / size)); i++) {
				total_ingresos += ingresos[i];
				total_gastos += gastos[i];
			}

			// Recibimos los mensajes con la suma, en este caso del cliente falta añadir el gasto
			for (int i = 1; i < size; i++) {
				int ingreso = 0, gasto = 0;
				MPI_Recv(&ingreso, 1, MPI_INT, i, 123, MPI_COMM_WORLD, &status);
				MPI_Recv(&gasto, 1, MPI_INT, i, 123, MPI_COMM_WORLD, &status);
				total_ingresos += ingreso;
				total_gastos += gasto;
			}

			diferencia_gastos_ingresos = total_gastos - total_ingresos;

			printf("Suma ingresos: %i\n", total_ingresos);
			printf("Suma gastos: %i\n", total_gastos);
			printf("La diferencia entre los gastos y los ingresos es: %i \n", diferencia_gastos_ingresos);
		
			free(ingresos);
			free(gastos);

		} else {
			MPI_Send(&ingresos[(numero_clientes / size) + 1], (numero_clientes / size) + 1, MPI_INT, 1, 123, MPI_COMM_WORLD);
			MPI_Send(&gastos[(numero_clientes / size) + 1], (numero_clientes / size) + 1, MPI_INT, 1, 123, MPI_COMM_WORLD);

			for (int i = 0; i < ((numero_clientes / size) + 1); i++) {
				total_ingresos += ingresos[i];
				total_gastos += gastos[i];
			}

			printf("\n \n");

			// Recibimos los mensajes con la suma, en este caso del cliente falta añadir el gasto
			for (int i = 1; i < size; i++) {
				int ingreso = 0, gasto = 0;
				MPI_Recv(&ingreso, 1, MPI_INT, i, 123, MPI_COMM_WORLD, &status);
				MPI_Recv(&gasto, 1, MPI_INT, i, 123, MPI_COMM_WORLD, &status);
				total_ingresos += ingreso;
				total_gastos += gasto;
			}

			diferencia_gastos_ingresos = total_gastos - total_ingresos;

			printf("Suma ingresos: %i\n", total_ingresos);
			printf("Suma gastos: %i\n", total_gastos);
			printf("La diferencia entre los gastos y los ingresos es: %i \n", diferencia_gastos_ingresos);
		
			free(ingresos);
			free(gastos);
		}

	} else if (rank == 1) {
		MPI_Recv(&numero_clientes,1,MPI_INT,0,123,MPI_COMM_WORLD,&status);

		if (isNumberEven(numero_clientes,size)) {
			ingresos = (int*) calloc((numero_clientes / size), sizeof(int));
			gastos = (int*) calloc((numero_clientes / size), sizeof(int));

			MPI_Recv(ingresos, (numero_clientes / size), MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
			MPI_Recv(gastos, (numero_clientes / size), MPI_INT, 0, 123, MPI_COMM_WORLD, &status);

			for (int i = 0; i < (numero_clientes / size); i++) {
				total_ingresos += ingresos[i];
				total_gastos += gastos[i];
			}

			MPI_Send(&total_ingresos, 1, MPI_INT, 0, 123, MPI_COMM_WORLD);
			MPI_Send(&total_gastos, 1, MPI_INT, 0, 123, MPI_COMM_WORLD);
		
			free(ingresos);
			free(gastos);
		} else {
			ingresos = (int*) calloc(((numero_clientes / size) + 1), sizeof(int));
			gastos = (int*) calloc(((numero_clientes / size) + 1), sizeof(int));

			MPI_Recv(ingresos, ((numero_clientes / size) + 1), MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
			MPI_Recv(gastos, ((numero_clientes / size) + 1), MPI_INT, 0, 123, MPI_COMM_WORLD, &status);

			for (int i = 0; i < (numero_clientes / size) + 1; i++) {
				total_ingresos += ingresos[i];
				total_gastos += gastos[i];
			}

			MPI_Send(&total_ingresos, 1, MPI_INT, 0, 123, MPI_COMM_WORLD);
			MPI_Send(&total_gastos, 1, MPI_INT, 0, 123, MPI_COMM_WORLD);
		
			free(ingresos);
			free(gastos);
		}

	} 

	MPI_Finalize();
	
	return 0;
}