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
    char letra;

    configuracion->portJack = 0;
    read(*conf, &letra, sizeof(char));
    configuracion->portJack = (configuracion->portJack) * 10 + letra - '0';
    read(*conf, &letra, sizeof(char));
    while (letra != '\n') {
        configuracion->portJack = (configuracion->portJack) * 10 + letra - '0';
        read(*conf, &letra, sizeof(char));
    }
}
