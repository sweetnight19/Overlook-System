all: jack

jack.o: jack.c Configuracion/configuracion.h Lloyd/lloyd.h Semaforo/semaphore.h
	@gcc -c jack.c -Wall -Wextra -lpthread -std=gnu99
	@echo "Compilando jack.c"

configuracion.o: Configuracion/configuracion.c
	@gcc -c Configuracion/configuracion.c -Wall -Wextra 
	@echo "Compilando configuracion.c"

servidor.o: Servidor/servidor.c Servidor/servidor.h Semaforo/semaphore.h Lloyd/lloyd.h
	@gcc -c Servidor/servidor.c -Wall -Wextra -lpthread
	@echo "Compilando servidor.c"

semaphore.o: Semaforo/semaphore.c Semaforo/semaphore.h
	@gcc -c Semaforo/semaphore.c -Wall -Wextra -lpthread
	@echo "Compilando semaforo.c"

lloyd.o: Lloyd/lloyd.c Lloyd/lloyd.h
	@gcc -c  Lloyd/lloyd.c -Wall -Wextra -lpthread
	@echo "Compilando lloyd.c"

jack: jack.o configuracion.o servidor.o semaphore.o lloyd.o
	@gcc jack.o configuracion.o servidor.o semaphore.o lloyd.o -Wall -Wextra -lpthread -o jack.exe
	@echo "Compilacion terminada!"

clean:
	@rm -f *.o