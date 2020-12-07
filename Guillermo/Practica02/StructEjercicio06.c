#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <mpi.h>

#define N 13

typedef struct Pais {
	char pais[20];
	char capital[20];
} paises;

int main(int argc, char* argv[]) {
	int size = 0;	// Número de procesadores 
	int rank = 0;	// Id del procesador
	char cadena[20];

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;

	MPI_Datatype MPI_PAIS;				// 	El nombre que contenga el nuevo tipo
	int campos = 2;						//	Número de atributos de la estructura
	int longitud_campo[] = {20, 20};	//	20 elementos de cada tipo interno
	MPI_Aint indices[2];				// 	Vector para registrar las posiciones relativas en memoria

	indices[0] = (MPI_Aint) offsetof(paises, pais);
	indices[1] = (MPI_Aint) offsetof(paises, capital);

	MPI_Datatype tipo_base[] = {MPI_CHAR, MPI_CHAR};

	MPI_Type_create_struct(campos, longitud_campo, indices, tipo_base, &MPI_PAIS);	//	Escribimos la información del tipo
	MPI_Type_commit(&MPI_PAIS);	//	Registrarlo 

	paises paises_totales[N];
		
	if (rank == 0) {
		strcpy(paises_totales[0].pais,"España");
		strcpy(paises_totales[0].capital,"Madrid");

		strcpy(paises_totales[1].pais,"Inglaterra");
		strcpy(paises_totales[1].capital,"Londres");

		strcpy(paises_totales[2].pais,"Francia");
		strcpy(paises_totales[2].capital,"París");

		strcpy(paises_totales[3].pais,"Alemania");
		strcpy(paises_totales[3].capital,"Bonn");

		strcpy(paises_totales[4].pais,"India");
		strcpy(paises_totales[4].capital,"Nueva Delhi");

		strcpy(paises_totales[5].pais,"Israel");
		strcpy(paises_totales[5].capital,"Jerusalén");

		strcpy(paises_totales[6].pais,"Italia");
		strcpy(paises_totales[6].capital,"Roma");

		strcpy(paises_totales[7].pais,"Japón");
		strcpy(paises_totales[7].capital,"Tokio");

		strcpy(paises_totales[8].pais,"Méjico");
		strcpy(paises_totales[8].capital,"Ciudad de Méjico");

		strcpy(paises_totales[9].pais,"China");
		strcpy(paises_totales[9].capital,"Pekín");

		strcpy(paises_totales[10].pais,"Rusi");
		strcpy(paises_totales[10].capital,"Moscú");

		strcpy(paises_totales[11].pais,"Estados Unidos");
		strcpy(paises_totales[11].capital,"Washington");

		strcpy(paises_totales[12].pais,"Canadá");
		strcpy(paises_totales[12].capital,"Ottwa");
	}

	MPI_Bcast(paises_totales, N, MPI_PAIS, 0, MPI_COMM_WORLD);

	if (rank != 0) {
		for (int i = 0; i < N; i++) {
			printf("Soy el procesador %i, esta es mi cadena %s %s\n",rank,paises_totales[i].pais,paises_totales[i].capital);
		}
	}


	MPI_Type_free(&MPI_PAIS);
	MPI_Finalize();
	return 0;
}
