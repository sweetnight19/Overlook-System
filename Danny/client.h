#ifndef DANNY_CLIENT_H
#define DANNY_CLIENT_H

//Librerias del sistema
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include "configuracion.h"

void configurarCliente(char IPJack,int portJack);

#endif