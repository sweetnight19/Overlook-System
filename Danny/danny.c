/**
 * @authors: David Marquet, Joan Casals
 */

//Librerias del sistema
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

//Librerias propias
#include "Configuracion/configuracion.h"


#define BUFFER 200

Configuracion *configuracion;

void signalHandler() {

    write(STDOUT_FILENO, "Disconnecting Danny...\n", sizeof("Disconnecting Danny...\n"));
    free(configuracion);
    raise(SIGINT);
}

int main(int argc, char *argv[]) {
    int conf;
    char buffer[BUFFER];

    signal(SIGINT, signalHandler);
    configuracion = (Configuracion *) malloc(sizeof(Configuracion));
    if (argc != 2) {
        write(STDOUT_FILENO, "ERROR: No has indicado el archivo de configuración\n",
              sizeof("ERROR: No has indicado el archivo de configuración\n"));
        free(configuracion);
        return EXIT_FAILURE;
    }
    conf = open(argv[1], O_RDONLY);
    if (conf < 0) {
        write(STDOUT_FILENO, "ERROR: No es correcto el path del archivo de configuración\n",
              sizeof("ERROR: No es correcto el path del archivo de configuración\n"));
    } else {
        lecturaConfiguracion(&conf, configuracion);
        write(STDOUT_FILENO, "\nStarting Danny...\n\n", sizeof(char) * strlen("\nStarting Danny...\n\n"));

        //Aqui comença un bucle
        sprintf(buffer, "$%s:\n", configuracion->nombre);
        write(STDOUT_FILENO, buffer, sizeof(char) * strlen(buffer));
        write(STDOUT_FILENO, "Testing...\n", sizeof(char) * strlen("Testing...\n"));
        /*
        execle("/bin/ls","ls","-l","-a",0,configuracion->path);
        int test;
        test=execlp("ls","ls","-af",configuracion->path,NULL);
        printf("test: %d\n",test);
         */
    }
    close(conf);
    free(configuracion);
    return EXIT_SUCCESS;
}