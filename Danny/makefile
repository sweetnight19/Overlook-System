all: danny

danny.o: danny.c configuracion.h client.h
	@gcc -c danny.c -Wall -Wextra
	@echo "Compilando danny.c"

configuracion.o: configuracion.c configuracion.h
	@gcc -c configuracion.c -Wall -Wextra
	@echo "Compilando configuracion.c"

client.o: client.c client.h
	@gcc -c client.c -Wall -Wextra
	@echo "Compilando client.client"

danny: danny.o configuracion.o client.o
	@gcc danny.o configuracion.o client.o -Wall -Wextra -o danny.exe
	@echo "Compilacion terminada!"

clean:
	@rm -f *.o