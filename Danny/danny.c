/**
 * @authors: David Marquet, Joan Casals
 */

//Librerias del sistema
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <dirent.h>

//Librerias propias
#include "configuracion.h"
#include "client.h"


#define BUFFER 300

Configuracion *configuracion;
Datos *datos;

void signalHandler() {

    write(STDOUT_FILENO, "\n\nDisconnecting Danny...\n", sizeof("\n\nDisconnecting Danny...\n"));
    free(configuracion->path);
    free(configuracion);
    free(datos);
    raise(SIGINT);
}

int main(int argc, char *argv[]) {
    int conf, numArchivos, txtfd, hayTXT;
    char buffer[BUFFER], archivoTXT[BUFFER];
    DIR *directorio = NULL;
    struct dirent *direntp;

    //Iniciamos las variables y la memoria dinamica
    numArchivos = hayTXT = 0;
    signal(SIGINT, signalHandler);
    configuracion = (Configuracion *) malloc(sizeof(Configuracion));
    datos = (Datos *) malloc(sizeof(Datos));

    //Comprobamos el argumento que sea correcto
    if (argc != 2) {
        write(STDOUT_FILENO, "ERROR: No has indicado el archivo de configuración\n",
              sizeof("ERROR: No has indicado el archivo de configuración\n"));
        free(configuracion);
        return EXIT_FAILURE;
    }

    conf = open(argv[1], O_RDONLY);
    //Comprobamos que exista el fichero de configuracion
    if (conf < 0) {
        write(STDOUT_FILENO, "ERROR: No es correcto el path del archivo de configuración\n",
              sizeof("ERROR: No es correcto el path del archivo de configuración\n"));
    } else {
        //Leemos el fichero de configuracion
        lecturaConfiguracion(&conf, configuracion);
        close(conf);
        write(STDOUT_FILENO, "\nStarting Danny...\n\n", sizeof(char) * strlen("\nStarting Danny...\n\n"));
        do {
            //Prompt
            sprintf(buffer, "$%s:\n", configuracion->nombre);
            write(STDOUT_FILENO, buffer, sizeof(char) * strlen(buffer));

            //Testing
            write(STDOUT_FILENO, "Testing...\n", sizeof(char) * strlen("Testing...\n"));

            //Comprobamos que exista el directorio
            directorio = opendir(configuracion->path);
            if (directorio == NULL) {
                write(STDOUT_FILENO, "No directory found\n", sizeof("No directory found\n"));
                sleep(configuracion->tiempo);
            } else {
                //Leemos el directorio
                numArchivos = 0;
                while ((direntp = readdir(directorio)) != NULL) {
                    //Ignoramos los ficheros "." y ".."
                    if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0) {

                    } else {
                        //Nos apuntamos si hemos leido un fichero .txt
                        if (direntp->d_name[strlen(direntp->d_name) - 1] == 't' &&
                            direntp->d_name[strlen(direntp->d_name) - 2] == 'x' &&
                            direntp->d_name[strlen(direntp->d_name) - 3] == 't' &&
                            direntp->d_name[strlen(direntp->d_name) - 4] == '.') {
                            numArchivos++;
                            hayTXT = 1;
                        }
                        //Nos apuntamos si hemos leido un fichero .jpg
                        if (direntp->d_name[strlen(direntp->d_name) - 1] == 'g' &&
                            direntp->d_name[strlen(direntp->d_name) - 2] == 'p' &&
                            direntp->d_name[strlen(direntp->d_name) - 3] == 'j' &&
                            direntp->d_name[strlen(direntp->d_name) - 4] == '.') {
                            numArchivos++;
                        }
                    }
                }
                //Comprobamos si hemos leido algun fichero
                if (numArchivos == 0) {
                    write(STDOUT_FILENO, "No files found\n", sizeof("No files found\n"));
                    sleep(configuracion->tiempo);
                } else {
                    sprintf(buffer, "%d files found\n", numArchivos);
                    write(STDOUT_FILENO, buffer, sizeof(char) * strlen(buffer));
                    closedir(directorio);

                    //Volvemos a abrir el directorio para mirar los nombres de los ficheros y guardar el path
                    directorio = opendir(configuracion->path);
                    strcpy(archivoTXT, configuracion->path);
                    archivoTXT[strlen(archivoTXT)] = '/';
                    while ((direntp = readdir(directorio)) != NULL) {
                        //Buscamos el fichero .txt para guardar el path con su nombre y printamos el nombre
                        if (hayTXT == 1 && direntp->d_name[strlen(direntp->d_name) - 1] == 't' &&
                            direntp->d_name[strlen(direntp->d_name) - 2] == 'x' &&
                            direntp->d_name[strlen(direntp->d_name) - 3] == 't' &&
                            direntp->d_name[strlen(direntp->d_name) - 4] == '.') {
                            strcat(archivoTXT, direntp->d_name);
                            sprintf(buffer, "%s\n", direntp->d_name);
                            write(STDOUT_FILENO, buffer, sizeof(char) * strlen(buffer));
                        }
                        //Si existen archivos .jpg, los printamos
                        if (direntp->d_name[strlen(direntp->d_name) - 1] == 'g' &&
                            direntp->d_name[strlen(direntp->d_name) - 2] == 'p' &&
                            direntp->d_name[strlen(direntp->d_name) - 3] == 'j' &&
                            direntp->d_name[strlen(direntp->d_name) - 4] == '.') {
                            sprintf(buffer, "%s\n", direntp->d_name);
                            write(STDOUT_FILENO, buffer, sizeof(char) * strlen(buffer));
                        }
                    }
                    //Comprobamos si hay el fichero .txt deseado
                    if (hayTXT == 0) {
                        write(STDOUT_FILENO, "No file .txt found\n", sizeof("No file .txt found\n"));
                        sleep(configuracion->tiempo);
                    }
                }
            }
        } while (numArchivos == 0 || hayTXT == 0);
        closedir(directorio);

        write(STDOUT_FILENO, "\n", sizeof("\n"));
        write(STDOUT_FILENO, archivoTXT, sizeof(char) * strlen(archivoTXT));
        write(STDOUT_FILENO, "\n", sizeof("\n"));
        txtfd = open(archivoTXT, O_RDONLY);
        if (txtfd < 0) {
            write(STDOUT_FILENO, "ERROR: No es correcto el path del archivo de configuración\n",
                  sizeof("ERROR: No es correcto el path del archivo de configuración\n"));
        } else {
            lecturaTXT(&txtfd, datos);
            close(txtfd);

            //Eliminamos el fichero .txt despues de la lectura
            remove(archivoTXT);
            write(STDOUT_FILENO, "\n", sizeof("\n"));
        }
        sprintf(buffer, "$%s:\n", configuracion->nombre);
        write(STDOUT_FILENO, buffer, sizeof(char) * strlen(buffer));
        write(STDOUT_FILENO, "\nEsperando una acción...\n", sizeof("\nEsperando una acción...\n"));
        configurarCliente(configuracion->IPJack,configuracion->portJack);
        while (1) {
        }
    }
    free(datos);
    free(configuracion->path);
    free(configuracion);
    return EXIT_SUCCESS;
}