    #include <stdio.h>
    #include <stdlib.h>
    #include <mpi.h>
    #include <time.h>
    #include "vectores.h"
    #include<math.h>

int main (int argc, char* argv[]) {
    int size = 0, rank = 0;
    int numero_clientes = 0;
    int clientes_core = 0;
    int resto = 0;
    int total_ingresos = 0;
    int total_gastos = 0;
    int diferencia_gastos_ingresos = 0;
    int *ingresos = NULL;
    int *gastos = NULL;
    int *rec_ing = NULL;
    int *rec_gas = NULL;
    int *tam = NULL;
    int *ini = NULL;
 
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

    tam = (int*)calloc(size,sizeof(int));
    ini = (int*)calloc(size,sizeof(int));
 
    if (rank == 0) {
 
        do {
            printf("Ingrese el número de clientes de la empresa: ");
            fflush(stdin);
            fflush(stdout);
            scanf("%i",&numero_clientes);
        } while (numero_clientes <= 0);
        
        clientes_core = numero_clientes / size;
        resto = numero_clientes % size;

        for(int i = 1; i < size; i++){
            MPI_Send(&clientes_core, 1, MPI_INT, i, 111, MPI_COMM_WORLD);
            MPI_Send(&resto, 1, MPI_INT, i, 111, MPI_COMM_WORLD);
            MPI_Send(&numero_clientes, 1, MPI_INT, i, 111, MPI_COMM_WORLD);

        }


    }


    if(rank != 0){
        MPI_Recv(&clientes_core, 1, MPI_INT, 0, 111, MPI_COMM_WORLD, &status);
        MPI_Recv(&resto, 1, MPI_INT, 0, 111, MPI_COMM_WORLD, &status);
        MPI_Recv(&numero_clientes, 1, MPI_INT, 0, 111, MPI_COMM_WORLD, &status);

    }
    
    int aux = 0;    
    for(int i = 0; i < size; i++){
        ini[i] = aux;
        
        tam[i] = clientes_core;
        if(resto != 0){
            tam[i]++;
            resto--;
        }
        aux = ini[i] + tam[i];
    }


    if(rank == 0){        
 
        ingresos = (int*) calloc(numero_clientes, sizeof(int));
        gastos = (int*) calloc(numero_clientes, sizeof(int));
 
        fillVector_int(ingresos,numero_clientes);
        fillVector_int(gastos,numero_clientes);
        printf("Vector ingresos\n");
        showVector_int(ingresos,numero_clientes);
        printf("Vector gastos\n");
        showVector_int(gastos,numero_clientes);
        printf("\n");        
    }

    rec_ing = (int*)calloc(tam[rank] , sizeof(int));
    rec_gas = (int*)calloc(tam[rank] , sizeof(int));    
    
    MPI_Scatterv(ingresos, tam, ini, MPI_INT, rec_ing, numero_clientes, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(gastos, tam, ini, MPI_INT, rec_gas, numero_clientes, MPI_INT, 0, MPI_COMM_WORLD);


   // print what each process received and compute 
    printf("Process %d: ", rank);
    for (int i = 0; i < tam[rank]; i++) {
        total_ingresos += rec_ing[i];
        total_gastos += rec_gas[i];
        rec_ing[i]*= 2;
        rec_gas[i]+=2;
    }

    printf("\n");
    if(rank!=0){
        MPI_Send(&total_ingresos, 1, MPI_INT, 0, 123, MPI_COMM_WORLD);
        MPI_Send(&total_gastos, 1, MPI_INT, 0, 123, MPI_COMM_WORLD);
    }

    if(rank == 0){      
        // Recibimos los mensajes con la suma, en este caso del cliente falta añadir el gasto
        for (int i = 1; i < size; i++) {
            int ingreso = 0, gasto = 0;
            MPI_Recv(&ingreso, 1, MPI_INT, i, 123, MPI_COMM_WORLD, &status);
            MPI_Recv(&gasto, 1, MPI_INT, i, 123, MPI_COMM_WORLD, &status);
            total_ingresos += ingreso;
            total_gastos += gasto;
        }
 
        diferencia_gastos_ingresos = total_gastos - total_ingresos;
 
        printf("Suma ingresos: %d\n", total_ingresos);
        printf("Suma gastos: %d\n", total_gastos);
        printf("La diferencia entre los gastos y los ingresos es: %d \n", abs(diferencia_gastos_ingresos));
        
        free(ingresos);
        free(gastos);
    }   
    
    
    free(rec_ing);
    free(rec_gas);
    free(tam);
    free(ini);
 
    MPI_Finalize();
    
    return 0;
}