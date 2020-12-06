#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <time.h>
#include "vectores.h"
#include<math.h>
#include<ctype.h>
#include<string.h>

int main(int argc, char** argv){
    int size, rank, opcion, n_str, aux, raiz, suma_3;
    double *vector, suma;
    char string[50];

    srand(time(NULL)); // Numero enteros aleatorios
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Indica el numero de nucleos disponible del procesador
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // ID del nucleo que estamos usando
    MPI_Status status; // Flag de finalización del proceso


    if(size != 4){
        printf("El programa debe ser lanzado para 4 procesadores\n!");
        MPI_Finalize();
        exit(0);
    }   

    if(rank == 0){
        do {
            printf("\n");
            printf("Por favor, seleccione una opción: \n"   
                "0: Finaliza la ejecución\n" 
                "1: Convertir texto a mayusculas\n" 
                "2: Vector de 10 numeros reales y su suma\n" 
                "3: Introducir mensaje y calcular su valor numérico\n" 
                "4: Realiza todo lo anterior a la vez\n"
                    
                "OPCION: ");

            fflush(stdout);
            scanf("%d", &opcion);
            
            for(int i = 1; i < size; i++){
                MPI_Send(&opcion, 1, MPI_INT, i, 123, MPI_COMM_WORLD);
            }

            if(opcion == 1 || opcion==4){
                printf("\n");
                printf("Introduzca el texto a convertir en mayusculas: ");
                fflush(stdout);
                scanf("%s", string);
                n_str = strlen(string);
                MPI_Send(&n_str, 1, MPI_INT, 1, 123, MPI_COMM_WORLD);
                MPI_Send(string, n_str, MPI_CHAR, 1, 123, MPI_COMM_WORLD);
                MPI_Recv(string, n_str, MPI_CHAR, 1, 123, MPI_COMM_WORLD, &status);

                printf("Texto en mayusculas:  %s\n", string);                
            } 
            fflush(stdin);
            fflush(stdout);

            
            if(opcion == 2 || opcion == 4){

                printf("\n");
                vector = (double *)malloc(10*sizeof(double));
                printf("Dado el siguiente vector: ");

                for(int i = 1; i <=10; i++){
                    vector[i-1] = (i!=10 ? i *1.1 : 10.10);
                    printf("%1.1f ", vector[i-1]);
                }
                MPI_Send(vector, 10, MPI_DOUBLE, 2,101, MPI_COMM_WORLD);
                MPI_Recv(&suma, 1, MPI_DOUBLE, 2, 123, MPI_COMM_WORLD, &status);
                MPI_Recv(&raiz, 1, MPI_INT, 2, 123, MPI_COMM_WORLD, &status);
                printf("\nLa suma es: %1.2f\n", suma);
                printf("La raiz cuadrada es %d\n", raiz);
            }      
                 
            if(opcion==3 || opcion == 4){
                printf("\n");
                MPI_Recv(&suma_3, 1, MPI_INT, 3, 123, MPI_COMM_WORLD, &status);
                printf("Entrando en la funcionalidad 3");
                printf("Suma de los valores númericos de los carácteres del mensaje anterior: %d\n", suma_3);
            }
        } while(opcion != 0);
        printf("Seleccionada opción 0, finalizando ejecución...");
        MPI_Finalize();
        exit(0);
    }

    if(rank == 1){
       
        do {
            MPI_Recv(&opcion, 1, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
            if(opcion==1 || opcion==4){

                MPI_Recv(&n_str, 1, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);

                MPI_Recv(string, n_str, MPI_CHAR, 0, 123, MPI_COMM_WORLD, &status);
                
                for (int indice = 0; string[indice] != '\0'; ++indice){
		            string[indice] = toupper(string[indice]);
	            }
                
                MPI_Send(string, n_str, MPI_CHAR, 0, 123, MPI_COMM_WORLD);
            }
        } while (opcion!= 0);
        MPI_Finalize();
        exit(0);
                
    } 
    if (rank == 2){
        do {    
            
            MPI_Recv(&opcion, 1, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
            if(opcion==2 || opcion==4){

                int n;
                MPI_Probe(0, 101, MPI_COMM_WORLD, &status);
                MPI_Get_count(&status, MPI_DOUBLE, &n);
                vector = (double*)malloc(n*sizeof(double));  

                MPI_Recv(vector, 10, MPI_DOUBLE, 0, 101, MPI_COMM_WORLD, &status);
                for(int i = 0; i <10; i++){
                    suma+=vector[i];
                }

                raiz = sqrt(suma);
                MPI_Send(&suma, 1, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD);
                MPI_Send(&raiz, 1, MPI_INT, 0, 123, MPI_COMM_WORLD);
                                
            }
        } while (opcion!= 0);
        MPI_Finalize();
        exit(0);
        
    }

    if (rank == 3){
        do {         
            MPI_Recv(&opcion, 1, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
            if(opcion==3 || opcion==4){

                suma_3 = 0;
                strcpy(string, "Entrando en funcionalidad 3");
                for (int i = 0; i < strlen(string); i++){
                    suma_3 += (int)string[i];
                }
            MPI_Send(&suma_3, 1, MPI_INT, 0, 123, MPI_COMM_WORLD);
            printf("\n");                 
            }
        } while (opcion!= 0);
        MPI_Finalize();
        exit(0);
        
    }
    
    
    MPI_Finalize();
    return 0;

}