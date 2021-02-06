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

int closeDanny, sockfd, sockfd2;

void signalHandler()
{
    write(STDOUT_FILENO, "\nDisconnecting Danny...\n", sizeof("\nDisconnecting Danny...\n"));
    write(STDOUT_FILENO, "\nComunicando desconnexion...\n", sizeof("\nnComunicando desconnexion...\n"));
    closeDanny = EXIT_FAILURE;
}

int main(int argc, char *argv[])
{
    int conf, /*conexionJack,*/ conexionWendy;
    Configuracion *configuracion;
    Datos *datos;

    //Iniciamos las variables y la memoria dinamica
    closeDanny = EXIT_SUCCESS;
    signal(SIGINT, signalHandler);
    configuracion = (Configuracion *)malloc(sizeof(Configuracion));
    datos = (Datos *)malloc(sizeof(Datos));
    //conexionJack = EXIT_SUCCESS;
    conexionWendy = EXIT_SUCCESS;

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
        write(STDOUT_FILENO, "\nStarting Danny...\n\n", sizeof(char) * strlen("\nStarting Danny...\n\n"));
        while (/*conexionJack == EXIT_SUCCESS &&*/ conexionWendy == EXIT_SUCCESS && closeDanny == EXIT_SUCCESS)
        {

            //Leemos el fichero de configuracion
            lecturaConfiguracion(&conf, configuracion);
            close(conf);

            //Conectamos con los servidores
            write(STDOUT_FILENO, "Connecting Jack...\n\n", sizeof(char) * strlen("Connecting Jack...\n\n"));
            //conexionJack = configurarCliente((char *)configuracion->IPJack, configuracion->portJack, &sockfd, configuracion->nombre);
            write(STDOUT_FILENO, "Connecting Wendy...\n\n", sizeof(char) * strlen("Connecting Wendy...\n\n"));
            conexionWendy = configurarCliente((char *)configuracion->IPWendy, configuracion->portWendy, &sockfd2, configuracion->nombre);
            if (/*conexionJack == EXIT_SUCCESS &&*/ conexionWendy == EXIT_SUCCESS)
            {

                while (closeDanny == EXIT_SUCCESS)
                {
                    //Leemos el fichero de datos y la imagen en el directorio indicado en el fichero de configuracion
                    comprobarFichero(configuracion, datos);

                    //Enviamos los datos al Jack
                    //enviarDatosJack(datos, &sockfd);
                    //write(STDOUT_FILENO, "Enviando datos al servidor Jack\n", sizeof(strlen("Enviando datos al servidor Jack\n")));

                    //Enviamos la imagen a Wendy
                    //enviarDatosWendy(datos, &sockfd2);
                    //write(STDOUT_FILENO, "Enviando imagen al servidor Wendy\n", sizeof(strlen("Enviando imagen al servidor Wendy\n")));

                    //Prompt
                    write(STDOUT_FILENO, "$", sizeof("$"));
                    write(STDOUT_FILENO, configuracion->nombre, sizeof(strlen(configuracion->nombre)));
                    write(STDOUT_FILENO, ":\n", sizeof(":\n"));
                    sleep(configuracion->tiempo);
                }
                //Desconnexion de Jack
                enviarTramaDesconec(&sockfd, configuracion->nombre);

                //Desconnexion de Wendy
                enviarTramaDesconec(&sockfd2, configuracion->nombre);

                close(sockfd);
                close(sockfd2);
            }
            else
            {
                write(STDOUT_FILENO, "\nJack and Wendy are not avaliable\n", sizeof("\nJack and Wendy are not avaliable\n"));
            }
        }
    }
    free(datos->imagenes.fotos);
    free(datos);
    free(configuracion->path);
    free(configuracion);
    return EXIT_SUCCESS;
}
