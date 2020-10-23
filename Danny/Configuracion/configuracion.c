#include "configuracion.h"

void lecturaConfiguracion(int *conf, Configuracion *configuracion) {
    int i;
    char letra, buffer[TEST];

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
    write(1, configuracion->nombre, sizeof(char) * strlen(configuracion->nombre));
}