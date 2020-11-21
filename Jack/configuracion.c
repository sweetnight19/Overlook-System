#include "configuracion.h"

void lecturaConfiguracion(int *conf, Configuracion *configuracion) {
    lecturaIPJack(conf, configuracion);
    lecturaPuertoJack(conf, configuracion);
}

void lecturaIPJack(int *conf, Configuracion *configuracion) {
    int i;
    char letra;

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
}

void lecturaPuertoJack(int *conf, Configuracion *configuracion) {
    int byte;
    char letra;

    configuracion->portJack = 0;
    read(*conf, &letra, sizeof(char));
    configuracion->portJack = (configuracion->portJack) * 10 + letra - '0';
    byte = read(*conf, &letra, sizeof(char));
    while (byte > 0 && (letra >= '0' && letra <= '9')) {
        configuracion->portJack = (configuracion->portJack) * 10 + letra - '0';
        byte = read(*conf, &letra, sizeof(char));
    }
}
