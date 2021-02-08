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
#include "Semaforo/semaphore.h"

int closeDanny, sockfd, sockfd2;
Configuracion *configuracion;
Datos *datos;
semaphore sem;

void signalHandler()
{
    write(STDOUT_FILENO, "\nDisconnecting Danny...\n", sizeof("\nDisconnecting Danny...\n"));
    write(STDOUT_FILENO, "\nComunicando desconnexion...\n", sizeof("\nnComunicando desconnexion...\n"));
    closeDanny = EXIT_FAILURE;
}

void alarmaHandler()
{
    SEM_wait(&sem);
    //Leemos el fichero de datos y la imagen en el directorio indicado en el fichero de configuracion
    comprobarFichero(configuracion, datos);
    signal(SIGALRM, alarmaHandler);
    alarm(configuracion->tiempo);
    SEM_signal(&sem);
}

int main(int argc, char *argv[])
{
    int conf, /*conexionJack,*/ conexionWendy;

    //Iniciamos las variables y la memoria dinamica
    closeDanny = EXIT_SUCCESS;
    signal(SIGINT, signalHandler);
    signal(SIGALRM, alarmaHandler);
    configuracion = (Configuracion *)malloc(sizeof(Configuracion));
    datos = (Datos *)malloc(sizeof(Datos));
    //conexionJack = EXIT_SUCCESS;
    conexionWendy = EXIT_SUCCESS;
    SEM_init(&sem, 1);

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
        write(STDOUT_FILENO, "\nStarting Danny...\n\n", sizeof("\nStarting Danny...\n\n"));
        while (/*conexionJack == EXIT_SUCCESS &&*/ conexionWendy == EXIT_SUCCESS && closeDanny == EXIT_SUCCESS)
        {

            //Leemos el fichero de configuracion
            lecturaConfiguracion(&conf, configuracion);
            alarm(configuracion->tiempo);
            close(conf);

            //Conectamos con los servidores
            write(STDOUT_FILENO, "Connecting Jack...\n\n", sizeof("Connecting Jack...\n\n"));
            //conexionJack = configurarCliente((char *)configuracion->IPJack, configuracion->portJack, &sockfd, configuracion->nombre);
            write(STDOUT_FILENO, "Connecting Wendy...\n\n", sizeof("Connecting Wendy...\n\n"));
            conexionWendy = configurarCliente((char *)configuracion->IPWendy, configuracion->portWendy, &sockfd2, configuracion->nombre);
            if (/*conexionJack == EXIT_SUCCESS &&*/ conexionWendy == EXIT_SUCCESS)
            {

                while (closeDanny == EXIT_SUCCESS && /*conexionJack == EXIT_SUCCESS &&*/ conexionWendy == EXIT_SUCCESS)
                {
                    //Enviamos los datos al Jack
                    //write(STDOUT_FILENO, "Enviando datos al servidor Jack\n", sizeof("Enviando datos al servidor Jack\n"));
                    //enviarDatosJack(datos, &sockfd);

                    if (datos->imagenes.numImagenes > 0)
                    {
                        SEM_wait(&sem);
                        //Enviamos la imagen a Wendy
                        write(STDOUT_FILENO, "Enviando imagenes al servidor Wendy\n", sizeof("Enviando imagenes al servidor Wendy\n"));
                        enviarDatosWendy(datos, &sockfd2);
                        SEM_signal(&sem);
                    }
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
    if (datos->imagenes.numImagenes > 0)
    {
        for (int i = 0; i < datos->imagenes.numImagenes; i++)
        {
            free(datos->imagenes.fotos[i].md5sum);
            free(datos->imagenes.fotos[i].mida);
        }
        free(datos->imagenes.fotos);
    }
    free(datos);
    free(configuracion->path);
    free(configuracion);
    return EXIT_SUCCESS;
}
