all: Jack

jack.o: jack.c configuracion.h servidor.h
	gcc -c jack.c

configuracion.o: configuracion.c configuracion.h
	gcc -c configuracion.c

servidor.o: servidor.c servidor.h
	gcc -c servidor.c

Jack: jack.o configuracion.o servidor.o
	gcc jack.o configuracion.o servidor.o -Wall -Wextra -o jack.exe