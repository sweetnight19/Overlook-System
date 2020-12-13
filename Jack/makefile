all: jack

jack.o: jack.c Configuracion/configuracion.h Servidor/servidor.h
	@gcc -c jack.c -Wall -Wextra -lpthread
	@echo "Compilando jack.c"

configuracion.o: Configuracion/configuracion.c Configuracion/configuracion.h
	@gcc -c Configuracion/configuracion.c -Wall -Wextra
	@echo "Compilando configuracion.c"

servidor.o: Servidor/servidor.c Servidor/servidor.h Semaforo/semaphore.h
	@gcc -c Servidor/servidor.c -Wall -Wextra -lpthread
	@echo "Compilando servidor.c"

jack: jack.o configuracion.o servidor.o
	@gcc jack.o configuracion.o servidor.o -Wall -Wextra -lpthread -o jack.exe
	@echo "Compilacion terminada!"

clean:
	@rm -f *.o