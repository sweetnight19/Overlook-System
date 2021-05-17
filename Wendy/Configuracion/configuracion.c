/**
 * @authors: David Marquet, Joan Casals
 */

#include "configuracion.h"

void lecturaConfiguracion(int *conf, Configuracion *configuracion) {
    lecturaIPJack(conf, configuracion);
    lecturaPuertoJack(conf, configuracion);
}

void lecturaIPJack(int *conf, Configuracion *configuracion) {
    int i = 0;
    char letra = ' ';

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
}

void lecturaPuertoJack(int *conf, Configuracion *configuracion) {
    int byte = 0;
    char letra = ' ';

    configuracion->portWendy = 0;
    read(*conf, &letra, sizeof(char));
    configuracion->portWendy = (configuracion->portWendy) * 10 + letra - '0';
    byte = read(*conf, &letra, sizeof(char));
    while (byte > 0 && (letra >= '0' && letra <= '9')) {
        configuracion->portWendy = (configuracion->portWendy) * 10 + letra - '0';
        byte = read(*conf, &letra, sizeof(char));
    }
}
