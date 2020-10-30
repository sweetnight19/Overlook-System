all: Jack

jack.o: jack.c configuracion.h
	gcc -c jack.c

configuracion.o: configuracion.c configuracion.h
	gcc -c configuracion.c

Jack: jack.o configuracion.o
	gcc jack.o configuracion.o -Wall -Wextra -o jack.exe