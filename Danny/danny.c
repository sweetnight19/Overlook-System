/**
 * @authors: David Marquet, Joan Casals
 */
//Librerias del sistema
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

//Librerias propias
#include "Configuracion/configuracion.h"
#include "Cliente/client.h"

Configuracion *configuracion;
Datos *datos;
int sockfd;

void signalHandler()
{
    write(STDOUT_FILENO, "\nDisconnecting Danny...\n", sizeof("\nDisconnecting Danny...\n"));

    free(configuracion->path);
    close(sockfd);
    free(configuracion);
    free(datos);
    raise(SIGINT);
}

int main(int argc, char *argv[])
{
    int conf, conexion;
    char buffer[BUFFER];

    //Iniciamos las variables y la memoria dinamica
    signal(SIGINT, signalHandler);
    configuracion = (Configuracion *)malloc(sizeof(Configuracion));
    datos = (Datos *)malloc(sizeof(Datos));
    conexion = EXIT_SUCCESS;

    //Comprobamos el argumento que sea correcto
    if (argc != 2)
    {
        write(STDOUT_FILENO, "ERROR: No has indicado el archivo de configuración\n",
              sizeof("ERROR: No has indicado el archivo de configuración\n"));
        free(configuracion);
        return EXIT_FAILURE;
    }

    conf = open(argv[1], O_RDONLY);
    //Comprobamos que exista el fichero de configuracion
    if (conf < 0)
    {
        write(STDOUT_FILENO, "ERROR: No es correcto el path del archivo de configuración\n", sizeof("ERROR: No es correcto el path del archivo de configuración\n"));
        return EXIT_FAILURE;
    }
    else
    {
        write(STDOUT_FILENO, "\nStarting Danny...\n", sizeof(char) * strlen("\nStarting Danny...\n\n"));
        while (conexion == EXIT_SUCCESS)
        {

            //Leemos el fichero de configuracion
            lecturaConfiguracion(&conf, configuracion);
            close(conf);

            //Conectamos con el servidor
            write(STDOUT_FILENO, "Connecting Jack...\n\n", sizeof(char) * strlen("Connecting Jack...\n\n"));
            conexion = configurarCliente((char *)configuracion->IPJack, configuracion->portJack, &sockfd, configuracion->nombre);

            if (conexion == EXIT_SUCCESS)
            {

                while (1)
                {

                    //Leemos el fichero .txt en el directorio indicado en el fichero de configuracion
                    comprobarFichero(configuracion, datos);

                    //Enviamos la trama
                    enviarDatos(datos, &sockfd);
                    //write(STDOUT_FILENO, "Data sent\n", sizeof(char) * strlen("Data sent\n"));
                    sprintf(buffer, "$%s:\n", configuracion->nombre);
                    write(STDOUT_FILENO, buffer, sizeof(char) * strlen(buffer));

                    sleep(configuracion->tiempo);
                }
            }
        }
    }
    close(sockfd);
    free(datos);
    free(configuracion->path);
    free(configuracion);
    return EXIT_SUCCESS;
}