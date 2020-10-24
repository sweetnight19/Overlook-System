/**
 * @authors: David Marquet, Joan Casals
 */

//Librerias del sistema
#include <fcntl.h>
#include <stdlib.h>

//Librerias propias
#include "Configuracion/configuracion.h"

int main(int argc, char *argv[]) {
    int conf;
    char buffer[NOMBRE];
    Configuracion *configuracion;

    configuracion = (Configuracion *) malloc(sizeof(Configuracion));
    if (argc != 2) {
        write(1, "ERROR: No has indicado el archivo de configuración\n",
              sizeof("ERROR: No has indicado el archivo de configuración\n"));
        free(configuracion);
        return 1;
    }
    conf = open(argv[1], O_RDONLY);
    if (conf < 0) {
        write(1, "ERROR: No es correcto el path del archivo de configuración\n",
              sizeof("ERROR: No es correcto el path del archivo de configuración\n"));
    } else {
        lecturaConfiguracion(&conf, configuracion);
        write(1, "\nStarting Danny...\n\n", sizeof(char) * strlen("\nStarting Danny...\n\n"));

        //Aqui comença un bucle
        sprintf(buffer, "$%s:\n", configuracion->nombre);
        write(1, buffer, sizeof(char) * strlen(buffer));
        write(1, "Testing...\n", sizeof(char) * strlen("Testing...\n"));
        /*
        int test;
        test=execlp("ls","ls","-af",configuracion->path,NULL);
        printf("test: %d\n",test);
         */
    }
    close(conf);
    free(configuracion);
    return 0;
}