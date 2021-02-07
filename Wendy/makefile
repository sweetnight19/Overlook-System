all: wendy

wendy.o: wendy.c Configuracion/configuracion.h Servidor/servidor.h
	@gcc -c wendy.c -Wall -Wextra -lpthread -std=gnu99
	@echo "Compilando wendy.c"

configuracion.o: Configuracion/configuracion.c
	@gcc -c Configuracion/configuracion.c -Wall -Wextra
	@echo "Compilando configuracion.c"

servidor.o: Servidor/servidor.c Servidor/servidor.h
	@gcc -c Servidor/servidor.c -std=gnu99 -Wall -Wextra -lpthread
	@echo "Compilando servidor.c"

wendy: wendy.o configuracion.o servidor.o 
	@gcc wendy.o configuracion.o servidor.o -Wall -Wextra -lpthread -o wendy.exe
	@echo "Compilacion terminada!"

clean:
	@rm -f *.o