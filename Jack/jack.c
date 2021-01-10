/**
 * @authors: David Marquet, Joan Casals
 */
//Librerias del sistema
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/mman.h>

//Librerias propias
#include "Configuracion/configuracion.h"
#include "Servidor/servidor.h"
#include "Lloyd/lloyd.h"
#include "Semaforo/semaphore.h"

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
    }else{

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
        }

        //INICIALITZEM ELS SEMAFORS I EL PROCES DE LLOYD

        Reg_estacions* reg_estacions;

        semaphore sem_read, sem_write;
        key_t key_read, key_write;
        int sem;

        key_read = ftok("read.txt", 'I');
        key_write = ftok("write.txt", 'D');

        sem = SEM_constructor_with_name(&sem_write, key_write);
        if (sem < 0)
        {
            write(STDOUT_FILENO, "\nERROR: No se ha podido crear el semaforo correctamente.\n", 
                sizeof(char) * strlen("\nERROR: No se ha podido crear el semaforo correctamente.\n"));
        }

        sem = SEM_constructor_with_name(&sem_read, key_read);
        if (sem < 0)
        {
            write(STDOUT_FILENO, "\nERROR: No se ha podido crear el semaforo correctamente.\n", 
                sizeof(char) * strlen("\nERROR: No se ha podido crear el semaforo correctamente.\n"));
        }

        SEM_init(&sem_write, 1);
        SEM_init(&sem_read, 0);

        //REALITZAR EL MAPEJAT DE LA MEMORIA REQUERIDA (com fer servir malloc)

        reg_estacions = mmap(NULL, sizeof(Reg_estacions), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

        switch(fork())
        {
            case -1: //HI HA HAGUT UN ERROR AL CREAR LLOYD
                
                write(STDOUT_FILENO, "\nERROR: No se ha podido crear el fork correctamente.\n", 
                    sizeof(char) * strlen("\nERROR: No se ha podido crear el fork correctamente.\n"));
                break;

            case 0: //ENTREM A LLOYD

                processarDades(reg_estacions, &sem_write, &sem_read);

                munmap(reg_estacions, sizeof(reg_estacions));

                break;
            
            default:

                configurarServidor(configuracion->portJack);

                wait(NULL);
            
                munmap(reg_estacions, sizeof(reg_estacions));
                break;
        }

        SEM_destructor(&sem_read);
        SEM_destructor(&sem_write);

        return EXIT_SUCCESS;
    }
}