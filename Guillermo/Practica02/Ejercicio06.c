/*

	Considere la siguiente lista de países y sus capitales:
			|********************************************************************|
			| 			España				|				Madrid				 |
			|********************************************************************|
			| 			Inglaterra			|				Londres				 |
			|********************************************************************|
			| 			Francia				|				París				 | 
			|********************************************************************|
			| 			Alemania			|				Bonn				 |
			|********************************************************************|
			| 			India				|				Nueva Delhi			 |
			|********************************************************************|
			| 			Israel				|				Jerusalén			 | 
			|********************************************************************|
			| 			Italia				|				Roma				 |
			|********************************************************************|
			| 			Japón				|				Tokio				 |
			|********************************************************************|
			| 			Méjico				|				Ciudad de Méjico	 |
			|********************************************************************|
			| 			China				|				Pekín				 |
			|********************************************************************|
			| 			Rusi				|				Moscú				 | 
			|********************************************************************|
			| 			Estados Unidos		|				Washington			 |
			|********************************************************************|
			| 			Canadá				|				Ottawa				 | 
			|********************************************************************|

	Escriba un programa interactivo que acepte el nombre de un país como entrada
	y escriba su correspondiente capital y viceversa en el menor tiempo posible.
	Diseñe el programa de modo que se ejecute repetidamente hasta que se introduzca
	la palabra Fin. Para el diseño del programa tenga en cuenta que el procesador
	central (aquel con identificador cero) es el que solicita al usuario el nombre
	del país de entrada con el usuario. Además, será también el que conoce los 
	datos y, por tanto, el encargado de distribuir la carga. El resultado final
	lo mostrará el procesador con identificador 1.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

#define N 13

struct Pais {
    const char* nombre_pais;
    const char* capital_del_pais;
} paises[N] = {{"España", "Madrid"},{"Inglaterra", "Londres"}, {"Francia", "París"}, {"Alemania", "Bonn"}, {"India", "Nueva Delhi"}, {"Israel", "Jerusalén"}, {"Italia", "Roma"},
                {"Japón", "Tokio"}, {"Méjico", "Ciudad de Méjico"}, {"China", "Pekín"}, {"Rusi", "Moscú"}, {"Estados Unidos", "Washintong"}, {"Canadá", "Ottawa"}};


int main(int argc, char argv[]) {
	char nombre_pais[20];
	int size = 0;
	int rank = 0;
	int *tam = NULL;
	int *ini = NULL;
	int numero_core = 0;
	int resto = 0;
	int auxiliar = 0; 
	int columnas = 0;
	int capital_pais[20];

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size); 	//	Indica el número de nucleos disponible del procesador
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); 	//	ID del núcleo que estamos usando
	MPI_Status status; 						//	Flag de finalización del proceso

	tam = (int*) calloc(size,sizeof(int));
	ini = (int*) calloc(size,sizeof(int));

	if (rank == 0) {
		numero_core = 13 / size;
		resto = 13, % size;

		for (int i = 0; i < size; i++) {
			MPI_Send(&numero_core,1,MPI_INT,i,100,MPI_COMM_WORLD);
			MPI_Send(&resto,1,MPI_INT,i,100,MPI_COMM_WORLD);
		}
	}

	if (rank != 0) {
		MPI_Recv(&numero_core,1,MPI_INT,0,100,MPI_COMM_WORLD,&status);
		MPI_Recv(&resto,1,MPI_INT,0,100,MPI_COMM_WORLD,&status);
	}
   
    int auxiliar = 0;    
    for(int i = 0; i < size; i++){
        ini[i] = auxiliar;        
        tam[i] = numero_core;
        if(resto != 0){
            tam[i]++;
            resto--;
        }
        aux = ini[i] + tam[i];
    }


	do {
		if (rank == 0) {
			printf("Introduce el nombre del país: ");
			fflush(stdin);
			fflush(stdout);
			fgets(nombre_pais,15,stdin);

			for (int i = 1; i < size; i++) {
				MPI_Send(&nombre_pais,15,MPI_INT,i,101,MPI_COMM_WORLD);
			}

			MPI_Scatterv(&paises,tam,ini,MPI_INT,);
			

		} else if (rank == 1) {
			MPI_Recv(&nombre_pais,15,MPI_INT,0,101,MPI_COMM_WORLD,&status);

			size_t len = strlen(nombre_pais); // Longitud de una cadena de texto
        	nombre_pais[len - 1] = '\0';

        	for (int i=0; i < N; i++) {
        		if (strstr(paises[i].nombre_pais, nombre_pais)) {
            		printf("\nEl nombre de la capital del pais %s es %s\n", nombre_pais, paises[i].capital_del_pais);
            	}
       		 }

       		 MPI_Recv(&nombre_pais,20,MPI_INT,)
		} else {
			MPI_Recv(&nombre_pais,15,MPI_INT,0,101,MPI_COMM_WORLD,&status);

			size_t len = strlen(nombre_pais); // Longitud de una cadena de texto
        	nombre_pais[len - 1] = '\0';

        	for (int i=0; i < N; i++) {
        		if (strstr(paises[i].nombre_pais, nombre_pais)) {
            		fflush(stdin);
            		fflush(stdout);
            		fgets(nombre_pais,20,stdin);
            		MPI_Send(&nombre_pais,20,MPI_INT,1,101,MPI_COMM_WORLD);
            	}
       		}
		}
	} while (!strstr(nombre_pais,"Fin"));

	free(nombre_pais);
	free(tam);
	free(ini);

	MPI_Finalize();

	return 0;
}
