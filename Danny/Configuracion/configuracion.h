#ifndef DANNY_CONFIGURACION_H
#define DANNY_CONFIGURACION_H

//Librerias del sistema
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

//Constantes
#define NOMBRE 100
#define TEST 500

//Tipos propios
typedef struct {
    char nombre[NOMBRE];
    char *path;
    int tiempo;
    char IPJack;
    int portJack;
    char IPWendy;
    int portWendy;
} Configuracion;

void lecturaConfiguracion(int *conf, Configuracion *configuracion);

#endif
