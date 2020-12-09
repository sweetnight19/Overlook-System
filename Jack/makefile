all: Jack

jack.o: jack.c configuracion.h servidor.h
	gcc -c jack.c -Wall -Wextra -lpthread

configuracion.o: configuracion.c configuracion.h
	gcc -c configuracion.c -Wall -Wextra

servidor.o: servidor.c servidor.h
	gcc -c servidor.c -Wall -Wextra -lpthread

Jack: jack.o configuracion.o servidor.o
	gcc jack.o configuracion.o servidor.o -Wall -Wextra -lpthread -o jack.exe