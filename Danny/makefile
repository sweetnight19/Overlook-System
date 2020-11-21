all: Danny

danny.o: danny.c configuracion.h
	gcc -c danny.c

configuracion.o: configuracion.c configuracion.h
	gcc -c configuracion.c

client.o: client.c client.h
	gcc -c client.c

Danny: danny.o configuracion.o client.o
	gcc danny.o configuracion.o client.o -Wall -Wextra -o danny.exe
