all: Danny

danny.o: danny.c configuracion.h client.h
	gcc -c danny.c -Wall -Wextra

configuracion.o: configuracion.c configuracion.h
	gcc -c configuracion.c -Wall -Wextra

client.o: client.c client.h
	gcc -c client.c -Wall -Wextra

Danny: danny.o configuracion.o client.o
	gcc danny.o configuracion.o client.o -Wall -Wextra -o danny.exe
