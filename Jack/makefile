all: Jack

jack.o: jack.c configuracion.h servidor.h
	gcc -c jack.c -Wall -Wextra

configuracion.o: configuracion.c configuracion.h
	gcc -c configuracion.c -Wall -Wextra

servidor.o: servidor.c servidor.h
	gcc -c servidor.c -Wall -Wextra

Jack: jack.o configuracion.o servidor.o
	gcc jack.o configuracion.o servidor.o -Wall -Wextra -o jack.exe