/**
 * @authors: David Marquet, Joan Casals
 */

//Librerias del sistema
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

//Librerias propias
#include "Configuracion/configuracion.h"

int main(int argc, char *argv[]) {
    int conf;
    Configuracion *configuracion;

    configuracion = NULL;
    if (argc != 2) {
        write(1, "ERROR: No has indicado el archivo de configuración\n",
              sizeof("ERROR: No has indicado el archivo de configuración\n"));
        return 1;
    }
    conf = open(argv[1], O_RDONLY);
    if (conf < 0) {
        write(1, "ERROR: No es correcto el path del archivo de configuración\n",
              sizeof("ERROR: No es correcto el path del archivo de configuración\n"));
    } else {
        /*
        char letra,buffer[TEST];
        printf("\n");
        write(1, argv[1], sizeof(argv[1])*strlen(argv[1]));
        read(conf, &letra, sizeof(char ));
        sprintf(buffer,"letra: %c",letra);
        write(1, buffer, sizeof(char )*strlen(buffer));
        */
        lecturaConfiguracion(&conf, configuracion);
    }
    close(conf);
    free(configuracion);
    return 0;
}