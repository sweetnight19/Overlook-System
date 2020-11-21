/**
 * @authors: David Marquet, Joan Casals
 */

//Librerias del sistema
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>


//Librerias propias
#include "configuracion.h"
#include "client.h"


Configuracion *configuracion;
Datos *datos;

void signalHandler() {

    write(STDOUT_FILENO, "\n\nDisconnecting Danny...\n", sizeof("\n\nDisconnecting Danny...\n"));
    free(configuracion->path);
    free(configuracion);
    free(datos);
    raise(SIGINT);
}

int main(int argc, char *argv[]) {
    int conf;
    char buffer[BUFFER];

    //Iniciamos las variables y la memoria dinamica
    signal(SIGINT, signalHandler);
    configuracion = (Configuracion *) malloc(sizeof(Configuracion));
    datos = (Datos *) malloc(sizeof(Datos));

    //Comprobamos el argumento que sea correcto
    if (argc != 2) {
        write(STDOUT_FILENO, "ERROR: No has indicado el archivo de configuración\n",
              sizeof("ERROR: No has indicado el archivo de configuración\n"));
        free(configuracion);
        return EXIT_FAILURE;
    }

    conf = open(argv[1], O_RDONLY);
    //Comprobamos que exista el fichero de configuracion
    if (conf < 0) {
        write(STDOUT_FILENO, "ERROR: No es correcto el path del archivo de configuración\n",
              sizeof("ERROR: No es correcto el path del archivo de configuración\n"));
    } else {
        //Leemos el fichero de configuracion
        lecturaConfiguracion(&conf, configuracion);
        close(conf);
        write(STDOUT_FILENO, "\nStarting Danny...\n\n", sizeof(char) * strlen("\nStarting Danny...\n\n"));

        //Leemos el fichero .txt en el directorio indicado en el fichero de configuracion
        comprobarFichero(configuracion, datos);
        sprintf(buffer, "$%s:\n", configuracion->nombre);
        write(STDOUT_FILENO, buffer, sizeof(char) * strlen(buffer));
        write(STDOUT_FILENO, "\nEsperando una acción...\n", sizeof("\nEsperando una acción...\n"));
        configurarCliente((char *) configuracion->IPJack, configuracion->portJack);
    }
    free(datos);
    free(configuracion->path);
    free(configuracion);
    return EXIT_SUCCESS;
}