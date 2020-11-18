#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "vectores.h"
#include<stdbool.h>
#include<math.h>

bool isOdd(int n){
    return ((n%2) == 0) ?  false :  true;
}

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
 
    if (size != 4) {
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

        MPI_Send(&numero_clientes,1,MPI_INT,1,12345,MPI_COMM_WORLD);
        MPI_Send(&numero_clientes,1,MPI_INT,2,12345,MPI_COMM_WORLD);
        MPI_Send(&numero_clientes,1,MPI_INT,3,12345,MPI_COMM_WORLD);

 
        ingresos = (int*) calloc(numero_clientes, sizeof(int));
        gastos = (int*) calloc(numero_clientes, sizeof(int));
 
        fillVector(ingresos,numero_clientes);
        fillVector(gastos,numero_clientes);
        printf("Vector ingresos\n");
        showVector(ingresos,numero_clientes);
        printf("Vector gastos\n");
        showVector(gastos,numero_clientes);
        printf("\n");
 
        // Enviamos los fragmentos de los vectores al core 1
        
        for(int i = 0; i < numero_clientes; i++){
            MPI_Send(&ingresos[i],1, MPI_INT, (i%3)+1, 123, MPI_COMM_WORLD);
            MPI_Send(&gastos[i],1, MPI_INT, (i%3)+1, 123, MPI_COMM_WORLD);
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
        printf("La diferencia entre los gastos y los ingresos es: %i \n", abs(diferencia_gastos_ingresos));
        
        free(ingresos);
        free(gastos);
 
    } else if (rank == 1) {
        MPI_Recv(&numero_clientes,1,MPI_INT,0,12345,MPI_COMM_WORLD,&status);
        if((numero_clientes%3)==1)numero_clientes++;

        int ingreso;
        int gasto;
        double index = (double)numero_clientes/((double)size-1);
        for(int i = 0; i < round(index); i++){
            MPI_Recv(&ingreso, 1, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
            MPI_Recv(&gasto, 1, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
            printf(" %d-%d ",rank, ingreso);
            total_ingresos += ingreso;
            total_gastos += gasto;
        }

        MPI_Send(&total_ingresos, 1, MPI_INT, 0, 123, MPI_COMM_WORLD);
        MPI_Send(&total_gastos, 1, MPI_INT, 0, 123, MPI_COMM_WORLD);

    } else if (rank == 2) {
       // if((numero_clientes%3)==2) numero_clientes++;
        MPI_Recv(&numero_clientes,1,MPI_INT,0,12345,MPI_COMM_WORLD,&status);

        int ingreso;
        int gasto;
        double index = (double)numero_clientes/((double)size-1);
        for(int i = 0; i < round(index); i++){
            MPI_Recv(&ingreso, 1, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
            MPI_Recv(&gasto, 1, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
            printf(" %d-%d ", rank, ingreso);
            total_ingresos += ingreso;
            total_gastos += gasto;
        }

        MPI_Send(&total_ingresos, 1, MPI_INT, 0, 123, MPI_COMM_WORLD);
        MPI_Send(&total_gastos, 1, MPI_INT, 0, 123, MPI_COMM_WORLD);

    } else if (rank == 3) {

        MPI_Recv(&numero_clientes,1,MPI_INT,0,12345,MPI_COMM_WORLD,&status);
        int ingreso;
        int gasto;

        for(int i = 0; i < numero_clientes/(size-1); i++){

            MPI_Recv(&ingreso, 1, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
            MPI_Recv(&gasto, 1, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
            printf(" %d-%d ", rank, ingreso);
            total_ingresos += ingreso;
            total_gastos += gasto;
        }

        MPI_Send(&total_ingresos, 1, MPI_INT, 0, 123, MPI_COMM_WORLD);
        MPI_Send(&total_gastos, 1, MPI_INT, 0, 123, MPI_COMM_WORLD);
    }   
 
    MPI_Finalize();
    
    return 0;
}