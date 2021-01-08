# Macros
CC	=	gcc
CFLAGS	=	-g 
SRC = Ejer1.c vectores.c vectores.h
OBJ = Ejer1.o vectores.o
LIBS	=	-lm
RM	=	rm -f

# Reglas explícitas
all: $(OBJ)
	$(CC)	$(CFLAGS) -o Programa1 $(OBJ)
clean:
	$(RM)	$(OBJ) Programa1

# Reglas implícitas
vectores.o: vectores.c vectores.h
Ejer1.o: Ejer1.c vectores.h