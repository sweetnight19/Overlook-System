all: danny

danny.o: danny.c Configuracion/configuracion.h Cliente/client.h
	@gcc -c danny.c -Wall -Wextra
	@echo "Compilando danny.c"

configuracion.o: Configuracion/configuracion.c Configuracion/configuracion.h
	@gcc -c Configuracion/configuracion.c -Wall -Wextra
	@echo "Compilando configuracion.c"

client.o: Cliente/client.c Cliente/client.h
	@gcc -c Cliente/client.c -Wall -Wextra
	@echo "Compilando client.client"

danny: danny.o configuracion.o client.o
	@gcc danny.o configuracion.o client.o -Wall -Wextra -o danny.exe
	@echo "Compilacion terminada!"

clean:
	@rm -f *.o