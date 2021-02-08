/**
 * @authors: David Marquet, Joan Casals
 */

#ifndef DANNY_SERVIDOR_H
#define DANNY_SERVIDOR_H

//Librerias del sistema
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>

//Librerias propias
#include "../Configuracion/configuracion.h"
#include "../Semaforo/semaphore.h"
#include "../Lloyd/lloyd.h"

//Constantes
#define ORIGEN 14
#define NOMBRE 100
#define NUM_CLIENTES 100
#define NOMBRE 100
#define PRECIPITACION 4
#define PRESSION 6
#define HUMEDAD 3
#define TEMPERATURA 5
#define HORA 8
#define FECHA 10

void configurarServidor(int portJack, int *cerrar, Reg_estacions *reg_estacions, semaphore *sem_read_1,semaphore *sem_write_1);
void *TareasServidor(void *socket_desc);
void processarLloyd(char buffer[TRAMA], char nom_estacio[NOMBRE]);

#endif