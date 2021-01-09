//@authors: David Marquet, Joan Casals

#include "lloyd.h"

semaphore sem;
bool flag;

/*-------------------------------------------
*
           CREACIO LLOYD I EL THREAD
*
-------------------------------------------*/

void *lloyd_thread(void *arg)
{
    Estacions *estructura_estacions, buffer_estructura_estacions;

    estructura_estacions = (Estacions *)arg;
    buffer_estructura_estacions.estacions = (Estacio *)malloc(sizeof(Estacio));

    int fd;
    char buffer[300];

    // INICIALITZACIO DEL THREAD

    while (flag == true)
    {
        fd = open("../Hallorant.txt", O_WRONLY | O_CREAT | O_TRUNC);

        if (fd < 0) // NO S'HA POGUT OBRIR EL FITXER DE LES ESTADISTIQUES DE LES ESTACIONS
        {
            write(STDOUT_FILENO, "\nERROR: No ha sido posible abrir el fichero de las estadisticas.\n",
                  sizeof(char) * strlen("\nERROR: No ha sido posible abrir el fichero de las estadisticas.\n"));
        }
        else
        {
            while (fd >= 0)
            {
                SEM_wait(&sem);

                //INCIALITZACIO DE BUFFER_ESTRUCTURA_ESTACIONS PER QUE MONTSERRAT NO ES QUEIXI
                // COPIA DE LES DADES DE LES ESTACIONS EN UNA ESTRUCTURA QUE ACTUA COM A BUFFER PER COPIAR-HO AL DOCUMENT

                for (int i = 0; i < estructura_estacions->num_estacions; i++)
                {
                    strcpy(buffer_estructura_estacions.estacions[i].mitjana_estacions.nom_estacio, " ");
                    strcpy(buffer_estructura_estacions.estacions[i].mitjana_estacions.nom_estacio, estructura_estacions->estacions[i].nom);

                    buffer_estructura_estacions.estacions[i].mitjana_estacions.humitat = 0.0;
                    buffer_estructura_estacions.estacions[i].mitjana_estacions.humitat = estructura_estacions->estacions[i].mitjana_estacions.humitat;

                    buffer_estructura_estacions.estacions[i].mitjana_estacions.temperatura = 0.0;
                    buffer_estructura_estacions.estacions[i].mitjana_estacions.temperatura = estructura_estacions->estacions[i].mitjana_estacions.temperatura;

                    buffer_estructura_estacions.estacions[i].mitjana_estacions.precipitacio = 0.0;
                    buffer_estructura_estacions.estacions[i].mitjana_estacions.precipitacio = estructura_estacions->estacions[i].mitjana_estacions.precipitacio;

                    buffer_estructura_estacions.estacions[i].mitjana_estacions.pressio_atmos = 0.0;
                    buffer_estructura_estacions.estacions[i].mitjana_estacions.pressio_atmos = estructura_estacions->estacions[i].mitjana_estacions.pressio_atmos;

                    sprintf(buffer, "%s:\nHumitat mitjana: %.2f,\nTemperatura mitjana: %.2f,\nPrecipitacio mitjana: %.2f,\nPressio atmosferica mitjana: %.2f\n\n",
                            buffer_estructura_estacions.estacions[i].nom,
                            buffer_estructura_estacions.estacions[i].mitjana_estacions.humitat,
                            buffer_estructura_estacions.estacions[i].mitjana_estacions.temperatura,
                            buffer_estructura_estacions.estacions[i].mitjana_estacions.precipitacio,
                            buffer_estructura_estacions.estacions[i].mitjana_estacions.pressio_atmos);
                    write(fd, buffer, sizeof(char) * strlen(buffer));
                }
                sleep(15);
            }
            
            SEM_signal(&sem);
            close(fd);
        }
    }

    return NULL;
}

/*-------------------------------------------
*
        PROCESSAMENT DE LES ESTACIONS
*
-------------------------------------------*/

void processarDades(Reg_estacions *reg_estacions, semaphore *sem_write, semaphore *sem_read)
{
    pthread_t thread;

    Estacions estructura_estacions;
    int int_thread;
    int posicio = -39;

    flag = true;

    //signal(SIGINT, signalHandler);

    //SEM_constructor(&sem);
    SEM_init(&sem, 1);

    estructura_estacions.num_estacions = 0;
    estructura_estacions.estacions = (Estacio *)malloc(sizeof(Estacio));

    int_thread = pthread_create(&thread, NULL, lloyd_thread, (void *)&estructura_estacions);

    if (int_thread < 0) //No s'ha pogut crear el thread que permet tractar les dades de les estacions
    {
        write(STDOUT_FILENO, "\nERROR: No se ha podido crear el thread para tratar las estaciones.\n",
              sizeof(char) * strlen("\nERROR: No se ha podido crear el thread para tratar las estaciones.\n"));
    }
    else
    { // INICIALITZACIO DEL THREAD EXITOSA

        do
        {
            SEM_wait(sem_read);

            for (int i = 0; i < estructura_estacions.num_estacions; i++)
            {
                if (strcmp(reg_estacions->nom_estacio, estructura_estacions.estacions[i].nom) == 0)
                {
                    posicio = i;
                }

                SEM_wait(&sem);

                if (posicio < 0) //SI NO S'HA TROBAT LA POSICIO DE LA ESTACIO DINS L'ARRAY, VOL DIR QUE NO EXISTEIX,
                                 //PER AIXO ES GUARDEN LES DADES DE LA NOVA ESTACIO DINS L'ARRAY
                {
                    estructura_estacions.num_estacions++;
                    estructura_estacions.estacions = (Estacio *)realloc(estructura_estacions.estacions,
                                                                        sizeof(Estacio) * estructura_estacions.num_estacions);
                    strcpy(estructura_estacions.estacions[estructura_estacions.num_estacions].nom, reg_estacions->nom_estacio);
                    estructura_estacions.estacions[estructura_estacions.num_estacions - 1].num_lectures++;
                    estructura_estacions.estacions[estructura_estacions.num_estacions - 1].mitjana_estacions.humitat =
                        reg_estacions->humitat;
                    estructura_estacions.estacions[estructura_estacions.num_estacions - 1].mitjana_estacions.temperatura =
                        reg_estacions->temperatura;
                    estructura_estacions.estacions[estructura_estacions.num_estacions - 1].mitjana_estacions.precipitacio =
                        reg_estacions->precipitacio;
                    estructura_estacions.estacions[estructura_estacions.num_estacions - 1].mitjana_estacions.pressio_atmos =
                        reg_estacions->pressio_atmos;
                }
                else
                {
                    estructura_estacions.estacions[posicio].mitjana_estacions.humitat = reg_estacions->humitat;
                    estructura_estacions.estacions[posicio].mitjana_estacions.temperatura = reg_estacions->temperatura;
                    estructura_estacions.estacions[posicio].mitjana_estacions.precipitacio = reg_estacions->precipitacio;
                    estructura_estacions.estacions[posicio].mitjana_estacions.pressio_atmos = reg_estacions->pressio_atmos;
                    estructura_estacions.estacions[posicio].num_lectures++;

                    posicio = -39;
                }

                SEM_signal(&sem);
                SEM_signal(sem_write);
            }
        } while (flag == true);

        int_thread = pthread_join(thread, NULL);

        if (int_thread < 0)
        {
            write(STDOUT_FILENO, "\nERROR: No se ha podido cerrar correctamente el thread.\n",
                  sizeof(char) * strlen("\nERROR: No se ha podido cerrar correctamente el thread.\n"));
        }

        SEM_destructor(&sem);
    }
}