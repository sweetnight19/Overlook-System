all: Danny

danny.o: danny.c configuracion.h
	gcc -c danny.c

configuracion.o: configuracion.c configuracion.h
	gcc -c configuracion.c

Danny: danny.o configuracion.o
	gcc danny.o configuracion.o -Wall -Wextra -o danny.exe