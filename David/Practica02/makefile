CC      = mpicc
LIBS = -lm
RM      = rm -f

ejercicio1: 
	$(CC) vectores.c ejercicio1.c -o ejercicio1  $(LIBS)
ejercicio2:
	$(CC) vectores.c ejercicio2.c -o ejercicio2  $(LIBS)

ejercicio3:
	$(CC) vectores.c ejercicio3.c -o ejercicio3  $(LIBS)

ejercicio4:
	$(CC) vectores.c Matrices.c ejercicio4.c -o ejercicio4  $(LIBS)

ejercicio5:
	$(CC) vectores.c ejercicio5.c -o ejercicio5  $(LIBS) 

ejercicio6:
	$(CC) vectores.c ejercicio6.c -o ejercicio6  $(LIBS)

ejercicio7_paralelo:
	$(CC) vectores.c ejercicio7_paralelo.c -o ejercicio7_paralelo $(LIBS)

ejercicio7_secuencial:
	gcc vectores.c ejercicio7_secuencial.c -o ejercicio7_secuencial $(LIBS)

clean:
	$(RM) ejercicio1
	$(RM) ejercicio2
	$(RM) ejercicio3
	$(RM) ejercicio4
	$(RM) ejercicio5
	$(RM) ejercicio6
	$(RM) ejercicio7_paralelo
	$(RM) ejercicio7_secuencial	
