/**
 * @authors: David Marquet, Joan Casals
 */

#ifndef DANNY_SERVIDOR_H
#define DANNY_SERVIDOR_H

//Librerias del sistema
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <signal.h>

//Librerias propias
#include "configuracion.h"

//Constantes
#define ORIGEN 14

void configurarServidor(int portJack);

#endif
