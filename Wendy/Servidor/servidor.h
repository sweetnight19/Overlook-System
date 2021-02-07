/**
 * @authors: David Marquet, Joan Casals
 */

#ifndef WENDY_SERVIDOR_H
#define WENDY_SERVIDOR_H

//Librerias del sistema
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>

//Librerias propias
#include "../Configuracion/configuracion.h"

//Constantes
#define ORIGEN 14
#define NUM_CLIENTES 100
#define NAMEFILE 30
#define MD5SUM 32
#define ESPERA 500

void configurarServidor(int portJack);
void signalHandler();
void *TareasServidor(void *socket_desc);
char *calcularMd5sum(char *path);

#endif