all: jack

jack.o: jack.c configuracion.h servidor.h
	@gcc -c jack.c -Wall -Wextra -lpthread
	@echo "Compilando jack.c"

configuracion.o: configuracion.c configuracion.h
	@gcc -c configuracion.c -Wall -Wextra
	@echo "Compilando configuracion.c"

servidor.o: servidor.c servidor.h
	@gcc -c servidor.c -Wall -Wextra -lpthread
	@echo "Compilando servidor.c"

jack: jack.o configuracion.o servidor.o
	@gcc jack.o configuracion.o servidor.o -Wall -Wextra -lpthread -o jack.exe
	@echo "Compilacion terminada!"

clean:
	@rm -f *.o