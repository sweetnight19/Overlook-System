/**
 * @authors: David Marquet, Joan Casals
 */
#ifndef DANNY_CONFIGURACION_H
#define DANNY_CONFIGURACION_H

//Librerias del sistema
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

//Constantes
#define NOMBRE 100
#define PRECIPITACION 4
#define PRESSION 6
#define HUMEDAD 3
#define TEMPERATURA 5
#define HORA 8
#define FECHA 10
#define IP 16
#define BUFFER 300
#define MD5SUM 32

//Tipos propios
typedef struct
{
    char nombre[NOMBRE];
    char *path;
    int tiempo;
    char IPJack[IP];
    int portJack;
    char IPWendy[IP];
    int portWendy;
} Configuracion;

typedef struct
{
    char path[BUFFER];
    char nomFoto[BUFFER];
    int mida;
    char md5sum[MD5SUM];
} Fotografia;

typedef struct
{
    Fotografia *fotos;
    int numImagenes;
} Imagen;

typedef struct
{
    char fecha[FECHA];
    char hora[HORA];
    char temperatura[TEMPERATURA];
    char humedad[HUMEDAD];
    char presionAtmosferica[PRESSION];
    char precipitacion[PRECIPITACION];
    Imagen imagenes;
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

void comprobarFichero(Configuracion *configuracion, Datos *datos);

#endif
