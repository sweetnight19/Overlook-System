all: danny

danny.o: danny.c Configuracion/configuracion.h Cliente/client.h Semaforo/semaphore.h
	@gcc -c danny.c -Wall -Wextra
	@echo "Compilando danny.c"

configuracion.o: Configuracion/configuracion.c Configuracion/configuracion.h
	@gcc -c Configuracion/configuracion.c -Wall -Wextra
	@echo "Compilando configuracion.c"

client.o: Cliente/client.c Cliente/client.h
	@gcc -c Cliente/client.c -std=gnu99 -Wall -Wextra
	@echo "Compilando client.c"

semaphore.o: Semaforo/semaphore.c Semaforo/semaphore.h
	@gcc -c Semaforo/semaphore.c -Wall -Wextra
	@echo "Compilando semaforo.c"

danny: danny.o configuracion.o client.o semaphore.o
	@gcc danny.o configuracion.o client.o semaphore.o -Wall -Wextra -o danny.exe
	@echo "Compilacion terminada!"

clean:
	@rm -f *.o