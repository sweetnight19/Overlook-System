/**
 * @authors: David Marquet, Joan Casals
 */
//Librerias del sistema
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

//Librerias propias
#include "Configuracion/configuracion.h"
#include "Servidor/servidor.h"

Configuracion *configuracion;

int main(int argc, char *argv[])
{
    int conf;

    configuracion = (Configuracion *)malloc(sizeof(Configuracion));

    //Comprobamos el argumento que sea correcto
    if (argc != 2)
    {
        write(STDOUT_FILENO, "ERROR: No has indicado el archivo de configuración\n",
              sizeof("ERROR: No has indicado el archivo de configuración\n"));
        free(configuracion);
        return EXIT_FAILURE;
    }

    //Comprobamos que exista el fichero de configuracion
    conf = open(argv[1], O_RDONLY);
    if (conf < 0)
    {
        write(STDOUT_FILENO, "ERROR: No es correcto el path del archivo de configuración\n",
              sizeof("ERROR: No es correcto el path del archivo de configuración\n"));
        return EXIT_FAILURE;
    }
    else
    {
        //Leemos el fichero de configuracion
        lecturaConfiguracion(&conf, configuracion);

        write(STDOUT_FILENO, "\nStarting Jack...\n\n", sizeof("\nStarting Jack...\n\n"));
        write(STDOUT_FILENO, "$Jack:\n", sizeof("$Jack:\n"));
        write(STDOUT_FILENO, "Waiting...\n", sizeof("Waiting...\n"));
        configurarServidor(configuracion->portJack);
    }
    return EXIT_SUCCESS;
}