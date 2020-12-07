#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h> 
#include <mpi.h>

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
    
        paises global_data [n_paises]= {
                {"España", "Madrid"},
                {"Inglaterra", "Londres"}, 
                {"Francia", "París"}, 
                {"Alemania", "Bonn"}, 

                {"India", "Nueva Delhi"}, 
                {"Israel", "Jerusalén"}, 
                {"Italia", "Roma"},

                {"Japón", "Tokio"}, 
                {"Méjico", "Ciudad de Méjico"}, 
                {"China", "Pekín"}, 
                
                {"Rusi", "Moscú"}, 
                {"Estados Unidos", "Washintong"}, 
                {"Canadá", "Ottawa"}
        };
    }

    paises local_data[tam[rank]];
    MPI_Scatterv(&global_data, tam, ini, MPI_PAIS, &local_data, tam[rank], MPI_PAIS, 0, MPI_COMM_WORLD);
      
    

        if(rank==0) {
		
			printf("Introduce el nombre del país: ");
			fflush(stdin);
			fflush(stdout);
			scanf("%s", nombre);
            for(int i = 1; i<size;i++){
                MPI_Send(&nombre, char_len, MPI_CHAR, i, 101, MPI_COMM_WORLD);
            }
        }
        if(rank!=0){

            MPI_Recv(&nombre, char_len, MPI_CHAR, 0, 101, MPI_COMM_WORLD, &status);

        }        
        

        for(int i = 0; i<tam[rank]; i++){                
            if(strcmp(nombre, local_data[i].nombre_pais) == 0){

                if(rank==1){
                    printf("\nProcesador %d, Pais: %s Capital: %s\n",rank, local_data[i].nombre_pais, local_data[i].capital_del_pais);
                    MPI_Send(&local_data[i].capital_del_pais, char_len, MPI_CHAR, 1, 12321, MPI_COMM_WORLD);

                   
                 
                }else{
                    
                    /*strcpy(aux[0].nombre_pais, local_data[i].nombre_pais);
                    strcpy(aux[0].capital_del_pais, local_data[i].capital_del_pais);
                    printf("\nProcesador %d, Pais: %s Capital: %s\n",rank, aux[0].nombre_pais, aux[0].capital_del_pais);
                    MPI_Send(&aux, 1, MPI_PAIS, 1, 123, MPI_COMM_WORLD);
                    strcpy(resultado, "si");
                    */
                    MPI_Send(&local_data[i].capital_del_pais, char_len, MPI_CHAR, 1, 12321, MPI_COMM_WORLD);

                }
            }else{
                MPI_Send("no", char_len, MPI_CHAR, 1, 12321, MPI_COMM_WORLD);
            }    
             
            
        }

        if(rank==1){
            for(int i = 0; i < size; i++){
                MPI_Recv(&nombre, char_len, MPI_CHAR, MPI_ANY_SOURCE, 12321, MPI_COMM_WORLD, &status);
                if(strcmp(nombre, "no")!=0){
                    printf("%s", nombre);   
                }
            }              
        }
        
    
      
    MPI_Type_free(&MPI_PAIS);
    free(tam);
    free(ini);

	MPI_Finalize();

	return 0;
}