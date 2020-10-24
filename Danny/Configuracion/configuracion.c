#include "configuracion.h"

void lecturaConfiguracion(int *conf, Configuracion *configuracion) {
    int i, byte;
    char letra;

    //Nombre de la estacion
    i = 0;
    read(*conf, &letra, sizeof(char));
    configuracion->nombre[i] = letra;
    i++;
    read(*conf, &letra, sizeof(char));
    while (letra != '\n') {
        configuracion->nombre[i] = letra;
        i++;
        read(*conf, &letra, sizeof(char));
    }
    configuracion->nombre[i] = '\0';

    //Path
    i = 0;
    read(*conf, &letra, sizeof(char));
    configuracion->path = (char *) malloc(sizeof(char));
    configuracion->path[i] = letra;
    i++;
    read(*conf, &letra, sizeof(char));
    while (letra != '\n') {
        configuracion->path = (char *) realloc(configuracion->path, sizeof(char) * (i + 1));
        configuracion->path[i] = letra;
        i++;
        read(*conf, &letra, sizeof(char));
    }
    configuracion->path[i] = '\0';

    //Tiempo de espera
    configuracion->tiempo = 0;
    read(*conf, &letra, sizeof(char));
    configuracion->tiempo = (configuracion->tiempo) * 10 + letra - '0';
    read(*conf, &letra, sizeof(char));
    while (letra != '\n') {
        configuracion->tiempo = (configuracion->tiempo) * 10 + letra - '0';
        read(*conf, &letra, sizeof(char));
    }

    //IPJack
    i = 0;
    read(*conf, &letra, sizeof(char));
    configuracion->IPJack[i] = letra;
    read(*conf, &letra, sizeof(char));
    i++;
    while (letra != '\n') {
        configuracion->IPJack[i] = letra;
        i++;
        read(*conf, &letra, sizeof(char));
    }
    configuracion->IPJack[i] = '\0';

    //Puerto del servidor Jack
    configuracion->portJack = 0;
    read(*conf, &letra, sizeof(char));
    configuracion->portJack = (configuracion->portJack) * 10 + letra - '0';
    read(*conf, &letra, sizeof(char));
    while (letra != '\n') {
        configuracion->portJack = (configuracion->portJack) * 10 + letra - '0';
        read(*conf, &letra, sizeof(char));
    }

    //IPWendy
    i = 0;
    read(*conf, &letra, sizeof(char));
    configuracion->IPWendy[i] = letra;
    read(*conf, &letra, sizeof(char));
    i++;
    while (letra != '\n') {
        configuracion->IPWendy[i] = letra;
        i++;
        read(*conf, &letra, sizeof(char));
    }
    configuracion->IPWendy[i] = '\0';

    //Puerto del servidor Wendy
    configuracion->portWendy = 0;
    read(*conf, &letra, sizeof(char));
    configuracion->portWendy = (configuracion->portWendy) * 10 + letra - '0';
    byte = read(*conf, &letra, sizeof(char));
    while (byte > 0 && (letra >= '0' && letra <= '9')) {
        configuracion->portWendy = (configuracion->portWendy) * 10 + letra - '0';
        byte = read(*conf, &letra, sizeof(char));
    }
}