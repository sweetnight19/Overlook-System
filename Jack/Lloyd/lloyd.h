#ifndef LLOYD_H
#define LLOYD_H

//Librerias del sistema
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

//Librerias propias
#include "../Semaforo/semaphore.h"
//#include "../Servidor/servidor.h"

//Constantes
#define NOM_ESTACIO 100
#define HALLORANT 120
#define BUFFER 500

//Tipos propios
typedef struct
{
    char nom_estacio[NOM_ESTACIO];
    float humitat;
    float temperatura;
    float precipitacio;
    float pressio_atmos;
} Reg_estacions;

typedef struct
{
    char nom[NOM_ESTACIO];
    int num_lectures;
    Reg_estacions mitjana_estacions;
} Estacio;

typedef struct
{
    int num_estacions;
    Estacio *estacions;
} Estacions;

/*
*Alarma que captura per anar escribint en el fitxer Hallorant les estadistiques
*/
void alarmHandler();

/*
* Proces principal de Lloyd
*/
void processarDades(Reg_estacions *reg_estacions, semaphore *sem_write, semaphore *sem_read, int *cerrar);

#endif