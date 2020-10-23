#include "configuracion.h"

void lecturaConfiguracion(int *conf, Configuracion *configuracion) {
    int i;
    char letra, buffer[TEST];

    i = 0;

    read(*conf, &letra, sizeof(letra));
    configuracion->nombre[i] = letra;
    i++;
    read(*conf, &letra, sizeof(letra));
    sprintf(buffer, "%c", letra);
    write(1, buffer, sizeof(buffer) * strlen(buffer));
    while (letra != '\n') {
        configuracion->nombre[i] = letra;
        i++;
        read(*conf, &letra, sizeof(letra));
        sprintf(buffer, "%c", letra);
        write(1, buffer, sizeof(buffer) * strlen(buffer));
    }
    configuracion->nombre[i] = '\0';
    write(1, configuracion->nombre, sizeof(configuracion->nombre));
}