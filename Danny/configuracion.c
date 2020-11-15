#include "configuracion.h"

void lecturaConfiguracion(int *conf, Configuracion *configuracion) {
    char letra;

    lecturaNom(conf, configuracion);
    read(*conf, &letra, sizeof(char));
    lecturaPath(conf, configuracion);
    lecturaTiempo(conf, configuracion);
    lecturaIPJack(conf, configuracion);
    lecturaPuertoJack(conf, configuracion);
    lecturaIPWendy(conf, configuracion);
    lecturaPuertoWendy(conf, configuracion);
}

void lecturaNom(int *conf, Configuracion *configuracion) {
    int i;
    char letra;

    //Nombre de la estacion
    i = 0;
    read(*conf, &letra, sizeof(char));
    configuracion->nombre[i] = letra;
    i++;
    read(*conf, &letra, sizeof(char));
    while (letra != '\n') {
        configuracion->nombre[i] = letra;
        i++;
        read(*conf, &letra, sizeof(char));
    }
    configuracion->nombre[i] = '\0';
}

void lecturaPath(int *conf, Configuracion *configuracion) {
    int i;
    char letra;

    i = 0;
    read(*conf, &letra, sizeof(char));
    configuracion->path = (char *) malloc(sizeof(char));
    configuracion->path[i] = letra;
    i++;
    read(*conf, &letra, sizeof(char));
    while (letra != '\n') {
        configuracion->path = (char *) realloc(configuracion->path, sizeof(char) * i);
        configuracion->path[i] = letra;
        i++;
        read(*conf, &letra, sizeof(char));
    }
    configuracion->path[i - 1] = '\0';
}

void lecturaTiempo(int *conf, Configuracion *configuracion) {
    char letra;

    configuracion->tiempo = 0;
    read(*conf, &letra, sizeof(char));
    configuracion->tiempo = (configuracion->tiempo) * 10 + letra - '0';
    read(*conf, &letra, sizeof(char));
    while (letra >= '0' && letra <= '9') {
        configuracion->tiempo = (configuracion->tiempo) * 10 + letra - '0';
        read(*conf, &letra, sizeof(char));
    }
}

void lecturaIPJack(int *conf, Configuracion *configuracion) {
    int i;
    char letra;

    i = 0;
    read(*conf, &letra, sizeof(char));
    configuracion->IPJack[i] = letra;
    read(*conf, &letra, sizeof(char));
    i++;
    while (letra != '\n') {
        configuracion->IPJack[i] = letra;
        i++;
        read(*conf, &letra, sizeof(char));
    }
    configuracion->IPJack[i] = '\0';
}

void lecturaPuertoJack(int *conf, Configuracion *configuracion) {
    char letra;

    configuracion->portJack = 0;
    read(*conf, &letra, sizeof(char));
    configuracion->portJack = (configuracion->portJack) * 10 + letra - '0';
    read(*conf, &letra, sizeof(char));
    while (letra != '\n') {
        configuracion->portJack = (configuracion->portJack) * 10 + letra - '0';
        read(*conf, &letra, sizeof(char));
    }
}

void lecturaIPWendy(int *conf, Configuracion *configuracion) {
    int i;
    char letra;

    i = 0;
    read(*conf, &letra, sizeof(char));
    configuracion->IPWendy[i] = letra;
    read(*conf, &letra, sizeof(char));
    i++;
    while (letra != '\n') {
        configuracion->IPWendy[i] = letra;
        i++;
        read(*conf, &letra, sizeof(char));
    }
    configuracion->IPWendy[i] = '\0';
}

void lecturaPuertoWendy(int *conf, Configuracion *configuracion) {
    int byte;
    char letra;

    configuracion->portWendy = 0;
    read(*conf, &letra, sizeof(char));
    configuracion->portWendy = (configuracion->portWendy) * 10 + letra - '0';
    byte = read(*conf, &letra, sizeof(char));
    while (byte > 0 && (letra >= '0' && letra <= '9')) {
        configuracion->portWendy = (configuracion->portWendy) * 10 + letra - '0';
        byte = read(*conf, &letra, sizeof(char));
    }
}

void lecturaTXT(int *txtfd, Datos *datos) {
    int i;
    char letra;

    lecturaFecha(txtfd, datos);
    lecturaHora(txtfd, datos);
    lecturaTemperatura(txtfd, datos);
    lecturaHumedad(txtfd, datos);
    lecturaAtmosferica(txtfd, datos);
    lecturaPrecipitacion(txtfd, datos);
}

void lecturaFecha(int *txtfd, Datos *datos) {
    int i;
    char letra;

    i = 0;
    read(*txtfd, &letra, sizeof(char));
    datos->fecha[i] = letra;
    i++;
    read(*txtfd, &letra, sizeof(char));
    while (letra != '\n') {
        datos->fecha[i] = letra;
        i++;
        read(*txtfd, &letra, sizeof(char));
    }
    datos->fecha[i] = '\0';
    write(STDOUT_FILENO,"\n",sizeof("\n"));
    write(STDOUT_FILENO,datos->fecha,sizeof(char)*strlen(datos->fecha));
}

void lecturaHora(int *txtfd, Datos *datos) {
    int i;
    char letra;

    i = 0;
    read(*txtfd, &letra, sizeof(char));
    datos->hora[i] = letra;
    i++;
    read(*txtfd, &letra, sizeof(char));
    while (letra != '\n') {
        datos->hora[i] = letra;
        i++;
        read(*txtfd, &letra, sizeof(char));
    }
    datos->hora[i] = '\0';
    write(STDOUT_FILENO,"\n",sizeof("\n"));
    write(STDOUT_FILENO,datos->hora,sizeof(char)*strlen(datos->hora));
}

void lecturaTemperatura(int *txtfd, Datos *datos) {
    int i;
    char letra;

    i = 0;
    read(*txtfd, &letra, sizeof(char));
    datos->temperatura[i] = letra;
    i++;
    read(*txtfd, &letra, sizeof(char));
    while (letra != '\n') {
        datos->temperatura[i] = letra;
        i++;
        read(*txtfd, &letra, sizeof(char));
    }
    datos->temperatura[i] = '\0';
    write(STDOUT_FILENO,"\n",sizeof("\n"));
    write(STDOUT_FILENO,datos->temperatura,sizeof(char)*strlen(datos->temperatura));
}

void lecturaHumedad(int *txtfd, Datos *datos) {
    int i;
    char letra;

    i = 0;
    read(*txtfd, &letra, sizeof(char));
    datos->humedad[i] = letra;
    i++;
    read(*txtfd, &letra, sizeof(char));
    while (letra != '\n') {
        datos->humedad[i] = letra;
        i++;
        read(*txtfd, &letra, sizeof(char));
    }
    datos->humedad[i] = '\0';
    write(STDOUT_FILENO,"\n",sizeof("\n"));
    write(STDOUT_FILENO,datos->humedad,sizeof(char)*strlen(datos->humedad));
}

void lecturaAtmosferica(int *txtfd, Datos *datos) {
    int i;
    char letra;

    i = 0;
    read(*txtfd, &letra, sizeof(char));
    datos->presionAtmosferica[i] = letra;
    i++;
    read(*txtfd, &letra, sizeof(char));
    while (letra != '\n') {
        datos->presionAtmosferica[i] = letra;
        i++;
        read(*txtfd, &letra, sizeof(char));
    }
    datos->presionAtmosferica[i] = '\0';
    write(STDOUT_FILENO,"\n",sizeof("\n"));
    write(STDOUT_FILENO,datos->presionAtmosferica,sizeof(char)*strlen(datos->presionAtmosferica));

}

void lecturaPrecipitacion(int *txtfd, Datos *datos){
    int i;
    char letra;

    i = 0;
    read(*txtfd, &letra, sizeof(char));
    datos->precipitacion[i] = letra;
    i++;
    read(*txtfd, &letra, sizeof(char));
    while (letra != '\n') {
        datos->precipitacion[i] = letra;
        i++;
        read(*txtfd, &letra, sizeof(char));
    }
    datos->precipitacion[i] = '\0';
    write(STDOUT_FILENO,"\n",sizeof("\n"));
    write(STDOUT_FILENO,datos->precipitacion,sizeof(char)*strlen(datos->precipitacion));
}
