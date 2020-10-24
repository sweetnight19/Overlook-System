#ifndef DANNY_CONFIGURACION_H
#define DANNY_CONFIGURACION_H

//Librerias del sistema
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Constantes
#define NOMBRE 100
#define IP 16

//Tipos propios
typedef struct {
    char nombre[NOMBRE];
    char *path;
    int tiempo;
    char IPJack[IP];
    int portJack;
    char IPWendy[IP];
    int portWendy;
} Configuracion;

void lecturaConfiguracion(int *conf, Configuracion *configuracion);

#endif
