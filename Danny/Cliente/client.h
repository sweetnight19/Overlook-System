/**
 * @authors: David Marquet, Joan Casals
 */
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
#include "../Configuracion/configuracion.h"

//Constantes
#define TRAMA 115
#define ESPERA 500

/*
* Configuracio del client
*/
int configurarCliente(char IPJack[IP], int portJack, int *sockfd, char *nombre);

/*
*Encarregada de enviar les dades al servidor Jack
*/
void enviarDatosJack(Datos *datos, int *sockfd);

/*
*Encarregada de enviar les imatges al servidor Wendy
*/
void enviarDatosWendy(Datos *datos, int *sockfd);

/*
* Envia la trama de desconexio al servidor indicat per el socket
*/
void enviarTramaDesconec(int *sockfd2, char *nombre);

#endif