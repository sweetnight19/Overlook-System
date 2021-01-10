/**
 * @authors: David Marquet, Joan Casals
 */

//Librerias del sistema
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

//Librerias propias
#include "Configuracion/configuracion.h"
#include "Servidor/servidor.h"

int main(int argc, char *argv[])
{
    int conf;
    Configuracion *configuracion;

    configuracion = (Configuracion *)malloc(sizeof(Configuracion));

    //Comprobamos el argumento que sea correcto
    if (argc != 2)
    {
        write(STDOUT_FILENO, "ERROR: No has indicado el archivo de configuración\n",
              sizeof("ERROR: No has indicado el archivo de configuración\n"));
        return EXIT_FAILURE;
    }
    else
    {
        //Comprobamos que exista el fichero de configuracion
        conf = open(argv[1], O_RDONLY);
        if (conf < 0)
        {
            write(STDOUT_FILENO, "ERROR: No es correcto el path del archivo de configuración\n",
                  sizeof("ERROR: No es correcto el path del archivo de configuración\n"));
            free(configuracion);
            return EXIT_FAILURE;
        }
        else
        {
            //Leemos el fichero de configuracion
            lecturaConfiguracion(&conf, configuracion);
            write(STDOUT_FILENO, "\nStarting Wendy...\n\n", sizeof("\nStarting Wendy...\n\n"));
            configurarServidor(configuracion->portWendy);
        }
        free(configuracion);
        return 0;
    }
}