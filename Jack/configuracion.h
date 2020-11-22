#ifndef DANNY_CONFIGURACION_H
#define DANNY_CONFIGURACION_H

//Librerias del sistema
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Constantes
#define IP 16
#define TRAMA 115

//Tipos propios
typedef struct {
    char IPJack[IP];
    int portJack;
} Configuracion;

void lecturaConfiguracion(int *conf, Configuracion *configuracion);
void lecturaIPJack(int *conf, Configuracion *configuracion);
void lecturaPuertoJack(int *conf, Configuracion *configuracion);

#endif
