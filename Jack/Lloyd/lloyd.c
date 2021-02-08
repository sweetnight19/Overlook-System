//@authors: David Marquet, Joan Casals

#include "lloyd.h"

semaphore sem;
Estacions estructura_estacions;

/*
*Alarma que captura per anar escribint en el fitxer Hallorant les estadistiques
*/
void alarmHandler()
{
    int fd;
    char buffer[BUFFER];
    fd = open("Hallorant.txt", O_CREAT | O_WRONLY, 0666);

    if (fd < 0)
    {
        write(STDOUT_FILENO, "\nERROR: No ha sido posible abrir el fichero de las estadisticas.\n",
              sizeof(char) * strlen("\nERROR: No ha sido posible abrir el fichero de las estadisticas.\n"));
    }
    else
    {
        SEM_wait(&sem);
        for (int i = 0; i < estructura_estacions.num_estacions; i++)
        {
            sprintf(buffer, "%s:\nHumedad avg: %.2f,\nTemperatura avg: %.2f,\nPrecipitacion avg: %.2f,\nPression atmosferica avg: %.2f\n\n",
                    estructura_estacions.estacions[i].nom,
                    estructura_estacions.estacions[i].mitjana_estacions.humitat,
                    estructura_estacions.estacions[i].mitjana_estacions.temperatura,
                    estructura_estacions.estacions[i].mitjana_estacions.precipitacio,
                    estructura_estacions.estacions[i].mitjana_estacions.pressio_atmos);
            write(fd, buffer, strlen(buffer));
        }
    }
    SEM_signal(&sem);
    close(fd);
    signal(SIGALRM, alarmHandler);
    alarm(HALLORANT);
}

/*
* Proces principal de Lloyd
*/
void processarDades(Reg_estacions *reg_estacions, semaphore *sem_write, semaphore *sem_read, int *cerrar)
{
    int posicio_estacio;

    signal(SIGALRM, alarmHandler);
    SEM_init(&sem, 1);
    alarm(HALLORANT);

    posicio_estacio = -1;
    estructura_estacions.num_estacions = 0;
    estructura_estacions.estacions = (Estacio *)malloc(sizeof(Estacio));
    do
    {
        SEM_wait(sem_read);
        for (int i = 0; i < estructura_estacions.num_estacions; i++)
        {
            if (strcmp(reg_estacions->nom_estacio, estructura_estacions.estacions[i].nom) == 0)
            {
                posicio_estacio = i;
            }
        }
        SEM_wait(&sem);

        if (posicio_estacio < 0)
        {

            estructura_estacions.num_estacions++;
            estructura_estacions.estacions = (Estacio *)realloc(estructura_estacions.estacions, sizeof(Estacio) * estructura_estacions.num_estacions);
            strcpy(estructura_estacions.estacions[estructura_estacions.num_estacions - 1].nom, reg_estacions->nom_estacio);
            estructura_estacions.estacions[estructura_estacions.num_estacions - 1].num_lectures = 1;
            estructura_estacions.estacions[estructura_estacions.num_estacions - 1].mitjana_estacions.temperatura = reg_estacions->temperatura;
            estructura_estacions.estacions[estructura_estacions.num_estacions - 1].mitjana_estacions.humitat = reg_estacions->humitat;
            estructura_estacions.estacions[estructura_estacions.num_estacions - 1].mitjana_estacions.pressio_atmos = reg_estacions->pressio_atmos;
            estructura_estacions.estacions[estructura_estacions.num_estacions - 1].mitjana_estacions.precipitacio = reg_estacions->precipitacio;
        }
        else
        {

            estructura_estacions.estacions[posicio_estacio].mitjana_estacions.temperatura = ((estructura_estacions.estacions[posicio_estacio].mitjana_estacions.temperatura * estructura_estacions.estacions[posicio_estacio].num_lectures) + reg_estacions->temperatura) / (estructura_estacions.estacions[posicio_estacio].num_lectures + 1);
            estructura_estacions.estacions[posicio_estacio].mitjana_estacions.humitat = ((estructura_estacions.estacions[posicio_estacio].mitjana_estacions.humitat * estructura_estacions.estacions[posicio_estacio].num_lectures) + reg_estacions->humitat) / (estructura_estacions.estacions[posicio_estacio].num_lectures + 1);
            estructura_estacions.estacions[posicio_estacio].mitjana_estacions.pressio_atmos = ((estructura_estacions.estacions[posicio_estacio].mitjana_estacions.pressio_atmos * estructura_estacions.estacions[posicio_estacio].num_lectures) + reg_estacions->pressio_atmos) / (estructura_estacions.estacions[posicio_estacio].num_lectures + 1);
            estructura_estacions.estacions[posicio_estacio].mitjana_estacions.precipitacio = ((estructura_estacions.estacions[posicio_estacio].mitjana_estacions.precipitacio * estructura_estacions.estacions[posicio_estacio].num_lectures) + reg_estacions->precipitacio) / (estructura_estacions.estacions[posicio_estacio].num_lectures + 1);
            estructura_estacions.estacions[posicio_estacio].num_lectures++;
        }
        posicio_estacio = -1;
        SEM_signal(&sem);
        SEM_signal(sem_write);
    } while (*cerrar == EXIT_SUCCESS);
    free(estructura_estacions.estacions);
    SEM_destructor(&sem);
}