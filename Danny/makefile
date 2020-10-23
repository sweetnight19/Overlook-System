all: Danny

danny.o: danny.c Configuracion/configuracion.h
	gcc danny.c

configuracion.o: Configuracion/configuracion.c Configuracion/configuracion.h
	gcc configuracion.c

Danny: danny.o configuracion.o
	gcc danny.o configuracion.o -Wall -Wextra -o danny.exe