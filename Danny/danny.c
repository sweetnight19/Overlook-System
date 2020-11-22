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
int sockfd;

void signalHandler() {
    int i;
    char buffer[TRAMA], danny[6];

    write(STDOUT_FILENO, "\nDisconnecting Danny...\n", sizeof("\nDisconnecting Danny...\n"));
    write(STDOUT_FILENO, "Disconnecting Jack...", sizeof("Disconnecting Jack..."));
    strcpy(danny, "DANNY");
    for (int j = 0; j < strlen(danny - 1); j++) {
        buffer[j] = danny[j];
    }
    for (int j = 1 + strlen(danny); j < 14; j++) {
        buffer[j] = '\0';
    }
    buffer[14] = 'Q';
    i = 15;
    for (int j = 0; i < TRAMA && j < strlen(configuracion->nombre); i++, j++) {
        buffer[i] = configuracion->nombre[j];
    }
    for (int j = i; j < TRAMA; j++) {
        buffer[j] = '\0';
    }
    write(sockfd, buffer, TRAMA);

    free(configuracion->path);
    close(sockfd);
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
        write(STDOUT_FILENO, "\nStarting Danny...\n", sizeof(char) * strlen("\nStarting Danny...\n\n"));
        //Leemos el fichero de configuracion
        lecturaConfiguracion(&conf, configuracion);
        close(conf);

        //Conectamos con el servidor
        write(STDOUT_FILENO, "Connecting Jack...\n\n", sizeof(char) * strlen("Connecting Jack...\n\n"));
        configurarCliente((char *) configuracion->IPJack, configuracion->portJack, &sockfd, configuracion->nombre);

        //Leemos el fichero .txt en el directorio indicado en el fichero de configuracion
        comprobarFichero(configuracion, datos);

        //Enviamos la trama
        write(STDOUT_FILENO, "Data sent\n", sizeof(char) * strlen("Data sent\n"));

        sprintf(buffer, "$%s:\n", configuracion->nombre);
        write(STDOUT_FILENO, buffer, sizeof(char) * strlen(buffer));
        while (1){

        }
    }
    free(datos);
    free(configuracion->path);
    free(configuracion);
    return EXIT_SUCCESS;
}