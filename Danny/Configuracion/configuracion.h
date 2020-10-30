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

typedef struct {
    char fecha[NOMBRE];
    char hora[NOMBRE];
    char temperatura[NOMBRE];
    char humedad[NOMBRE];
    char presionAtmosferica[NOMBRE];
    char precipitacion[NOMBRE];
} Datos;

void lecturaConfiguracion(int *conf, Configuracion *configuracion);

void lecturaTXT(int *txtfd, Datos *datos);

void lecturaNom(int *conf, Configuracion *configuracion);

void lecturaPath(int *conf, Configuracion *configuracion);

void lecturaTiempo(int *conf, Configuracion *configuracion);

void lecturaIPJack(int *conf, Configuracion *configuracion);

void lecturaPuertoJack(int *conf, Configuracion *configuracion);

void lecturaIPWendy(int *conf, Configuracion *configuracion);

void lecturaPuertoWendy(int *conf, Configuracion *configuracion);

void lecturaFecha(int *txtfd, Datos *datos);

void lecturaHora(int *txtfd, Datos *datos);

void lecturaTemperatura(int *txtfd, Datos *datos);

void lecturaHumedad(int *txtfd, Datos *datos);

void lecturaAtmosferica(int *txtfd, Datos *datos);

void lecturaPrecipitacion(int *txtfd, Datos *datos);

#endif
