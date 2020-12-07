#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h> 
#include <mpi.h>
#include<stdbool.h>

#define n_paises 13
#define char_len 20

typedef struct Pais {
    char nombre_pais[char_len];
    char capital_del_pais[char_len];
}paises;
                


int main(int argc, char **argv) {
	char nombre[char_len];
	int size = 0;
	int rank = 0;
	int *tam = NULL;
	int *ini = NULL;
	int numero_core = 0;
	int resto = 0;
	char capital[char_len];
    char resultado[char_len];
    paises global_data;
    int fin;


	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size); 	//	Indica el número de nucleos disponible del procesador
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); 	//	ID del núcleo que estamos usando
	MPI_Status status; 						//	Flag de finalización del proceso


    MPI_Datatype MPI_PAIS;
    int campos = 2;
    int longuitud_campo[] = {char_len, char_len};
    MPI_Aint indices[2];
            
    indices[0] = (MPI_Aint) offsetof(paises, nombre_pais);
	indices[1] = (MPI_Aint) offsetof(paises, capital_del_pais);

    MPI_Datatype tipos_base[] = {MPI_CHAR, MPI_CHAR};
    MPI_Type_create_struct(campos, longuitud_campo, indices, tipos_base, &MPI_PAIS);
    MPI_Type_commit(&MPI_PAIS);

	tam = (int*) calloc(size,sizeof(int));
	ini = (int*) calloc(size,sizeof(int));

	if (rank == 0) {
		numero_core = n_paises / size;
		resto = n_paises % size;

		for (int i = 0; i < size; i++) {
			MPI_Send(&numero_core, 1, MPI_INT, i, 100, MPI_COMM_WORLD);
			MPI_Send(&resto, 1, MPI_INT, i, 100, MPI_COMM_WORLD);
		}
	}

	if (rank != 0) {
		MPI_Recv(&numero_core,1,MPI_INT,0,100,MPI_COMM_WORLD,&status);
		MPI_Recv(&resto,1,MPI_INT,0,100,MPI_COMM_WORLD,&status);
	}
   
    int aux = 0;    
    for(int i = 0; i < size; i++){
        ini[i] = aux;        
        tam[i] = numero_core;
        if(resto != 0){
            tam[i]++;
            resto--;
        }
        aux = ini[i] + tam[i];
    }

    if (rank == 0) {
    
        paises global_data[n_paises];
        strcpy(global_data[0].nombre_pais,"España");
		strcpy(global_data[0].capital_del_pais,"Madrid");

		strcpy(global_data[1].nombre_pais,"Inglaterra");
		strcpy(global_data[1].capital_del_pais,"Londres");

		strcpy(global_data[2].nombre_pais,"Francia");
		strcpy(global_data[2].capital_del_pais,"París");

		strcpy(global_data[3].nombre_pais,"Alemania");
		strcpy(global_data[3].capital_del_pais,"Bonn");

		strcpy(global_data[4].nombre_pais,"India");
		strcpy(global_data[4].capital_del_pais,"NuevaDelhi");

		strcpy(global_data[5].nombre_pais,"Israel");
		strcpy(global_data[5].capital_del_pais,"Jerusalén");

		strcpy(global_data[6].nombre_pais,"Italia");
		strcpy(global_data[6].capital_del_pais,"Roma");

		strcpy(global_data[7].nombre_pais,"Japón");
		strcpy(global_data[7].capital_del_pais,"Tokio");

		strcpy(global_data[8].nombre_pais,"Méjico");
		strcpy(global_data[8].capital_del_pais,"CiudadDeMéjico");

		strcpy(global_data[9].nombre_pais,"China");
		strcpy(global_data[9].capital_del_pais,"Pekín");

		strcpy(global_data[10].nombre_pais,"Rusi");
		strcpy(global_data[10].capital_del_pais,"Moscú");

		strcpy(global_data[11].nombre_pais,"EstadosUnidos");
		strcpy(global_data[11].capital_del_pais,"Washington");

		strcpy(global_data[12].nombre_pais,"Canadá");
		strcpy(global_data[12].capital_del_pais,"Ottwa");
    }

    paises local_data[tam[rank]];
    MPI_Scatterv(&global_data, tam, ini, MPI_PAIS, &local_data, tam[rank], MPI_PAIS, 0, MPI_COMM_WORLD);
      
    
    do{

        MPI_Barrier(MPI_COMM_WORLD);

        if(rank==0) {
		
			printf("Introduce el nombre o capital del país: ");
			fflush(stdin);
			fflush(stdout);
			scanf("%s", nombre);
            for(int i = 1; i<size;i++){
                MPI_Send(&nombre, char_len, MPI_CHAR, i, 101, MPI_COMM_WORLD);
            }
            printf("\n");
        }
        
        if(rank!=0){

            MPI_Recv(&nombre, char_len, MPI_CHAR, 0, 101, MPI_COMM_WORLD, &status);

        }        
        
        bool encontrado = false;
        for(int i = 0; i<tam[rank]; i++){                
            if(strcmp(nombre, local_data[i].nombre_pais) == 0){
                encontrado = true;
                MPI_Send(&local_data[i].capital_del_pais, char_len, MPI_CHAR, 1, 12321, MPI_COMM_WORLD);
            }
   
            if(strcmp(nombre, local_data[i].capital_del_pais) == 0){
                encontrado = true;
                MPI_Send(&local_data[i].nombre_pais, char_len, MPI_CHAR, 1, 12321, MPI_COMM_WORLD);
            }            
        }

        if(!encontrado){
            MPI_Send("no", char_len, MPI_CHAR, 1, 12321, MPI_COMM_WORLD);            
        }


        MPI_Barrier(MPI_COMM_WORLD);

        
        if(rank==1){
            if(strcmp(nombre, "fin")!=0){
                for(int i = 0; i<size; i++){
                    MPI_Recv(&resultado, char_len, MPI_CHAR, MPI_ANY_SOURCE, 12321, MPI_COMM_WORLD, &status);
                    if(strcmp(resultado, "no") != 0){
                        printf("\n%s -> %s\n",nombre, resultado);
                    }
                }
            }

        }

        
    }while(strcmp(nombre,"fin")!=0);
    
      
    MPI_Type_free(&MPI_PAIS);
    free(tam);
    free(ini);

	MPI_Finalize();

	return 0;
}