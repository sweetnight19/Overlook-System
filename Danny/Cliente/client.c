/**
 * @authors: David Marquet, Joan Casals
 */
#include "client.h"

/*
* Configuracio del client
*/
int configurarCliente(char IPJack[IP], int portJack, int *sockfd, char *nombre) {
    int i;
    struct sockaddr_in servaddr;
    char danny[6], buffer[TRAMA];

    strcpy(danny, "DANNY");
    danny[strlen(danny)] = '\0';

    // socket create and varification
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd == -1) {
        return EXIT_FAILURE;
    } else {
        bzero(&servaddr, sizeof(servaddr));

        // assign IP, PORT
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr(IPJack);
        servaddr.sin_port = htons(portJack);

        // connect the client socket to server socket
        if (connect(*sockfd, (void *) &servaddr, sizeof(servaddr)) != 0) {
            return EXIT_FAILURE;
        } else {
            for (int j = 0; j < (int) strlen("DANNY"); j++) {
                buffer[j] = danny[j];
            }
            for (int j = (int) strlen(buffer); j < 14; j++) {
                buffer[j] = '\0';
            }

            buffer[14] = 'C';
            i = 15;
            for (int j = 0; j < (int) strlen(nombre); i++, j++) {
                buffer[i] = nombre[j];
            }
            for (int j = i; j < TRAMA; j++) {
                buffer[j] = '\0';
            }

            //Enviamos trama de conexion
            write(*sockfd, buffer, TRAMA);
            read(*sockfd, buffer, TRAMA);
            for (int j = 15; buffer[j] != '\0'; j++) {
                write(STDOUT_FILENO, &buffer[j], sizeof(char));
            }
            write(STDOUT_FILENO, "\n\n", sizeof("\n\n"));
        }
        return EXIT_SUCCESS;
    }
}

/*
*Encarregada de enviar les imatges al servidor Wendy
*/
void enviarDatosWendy(Datos *datos, int *sockfd) {
    int j, fd, num_foto, byte;
    char buffer[TRAMA], danny[6], lectura[100];

    num_foto = 0;
    while (num_foto < datos->imagenes.numImagenes) {
        strcpy(danny, "DANNY");
        danny[strlen(danny)] = '\0';

        for (int j = 0; j < (int) strlen("DANNY"); j++) {
            buffer[j] = danny[j];
        }
        for (int j = (int) strlen(buffer); j < 14; j++) {
            buffer[j] = '\0';
        }
        buffer[14] = 'I';
        j = 15;
        for (int i = 0; i < (int) strlen(datos->imagenes.fotos[num_foto].nomFoto); i++, j++) {
            buffer[j] = datos->imagenes.fotos[num_foto].nomFoto[i];
        }
        //j++;
        buffer[j] = '#';
        j++;
        for (int i = 0; i < (int) strlen(datos->imagenes.fotos[num_foto].mida); i++, j++) {
            buffer[j] = datos->imagenes.fotos[num_foto].mida[i];
        }
        j++;
        buffer[j] = '#';
        j++;
        for (int i = 0; i < MD5SUM; i++, j++) {
            buffer[j] = datos->imagenes.fotos[num_foto].md5sum[i];
        }
        j++;
        while (j < TRAMA) {
            buffer[j] = '\0';
            j++;
        }
        //Enviado trama de informacion
        write(*sockfd, buffer, TRAMA);

        usleep(ESPERA);
        buffer[14] = 'F';
        fd = open(datos->imagenes.fotos[num_foto].path, O_RDONLY);
        byte = atoi(datos->imagenes.fotos[num_foto].mida);
        for (int k = 0; byte > 100; k++) {
            byte -= 100;
            read(fd, lectura, 100);
            j = 15;
            for (int i = 0; i < 100; i++, j++) {
                buffer[j] = lectura[i];
            }
            write(*sockfd, buffer, TRAMA);
            usleep(ESPERA);
        }
        read(fd, lectura, byte);
        j = 15;
        for (int i = 0; i < byte; i++, j++) {
            buffer[j] = lectura[i];
        }
        write(*sockfd, buffer, TRAMA);
        close(fd);
        write(STDOUT_FILENO, "Imagen enviada: ", sizeof("Imagen enviada: "));
        write(STDOUT_FILENO, datos->imagenes.fotos[num_foto].nomFoto, sizeof(datos->imagenes.fotos[num_foto].nomFoto));
        write(STDOUT_FILENO, "\n", sizeof("\n"));
        remove(datos->imagenes.fotos[num_foto].path);
        num_foto++;

        //Respueta de Wendy
        read(*sockfd, buffer, TRAMA);
        for (int j = 15; buffer[j] != '\0'; j++) {
            write(STDOUT_FILENO, &buffer[j], sizeof(char));
        }
        write(STDOUT_FILENO, "\n\n", sizeof("\n\n"));
    }
    for (int i = 0; i < datos->imagenes.numImagenes; i++) {
        free(datos->imagenes.fotos[i].md5sum);
        free(datos->imagenes.fotos[i].mida);
    }
    free(datos->imagenes.fotos);
    datos->imagenes.numImagenes = 0;
}

/*
*Encarregada de enviar les dades al servidor Jack
*/
void enviarDatosJack(Datos *datos, int *sockfd) {
    int j;
    char danny[6], buffer[TRAMA];

    for (int i = 0; i < TRAMA; i++) {
        buffer[i] = '\0';
    }

    strcpy(danny, "DANNY");
    for (int i = 0; i < (int) strlen(danny); i++) {
        buffer[i] = danny[i];
    }
    for (int i = (int) strlen(danny); i < 14; i++) {
        buffer[i] = '\0';
    }
    buffer[14] = 'D';
    j = 15;
    for (int i = 0; i < FECHA; i++, j++) {
        buffer[j] = datos->fecha[i];
    }
    buffer[j] = '#';
    j++;
    for (int i = 0; i < HORA; i++, j++) {
        buffer[j] = datos->hora[i];
    }
    buffer[j] = '#';
    j++;
    for (int i = 0; i < TEMPERATURA - 1; i++, j++) {
        buffer[j] = datos->temperatura[i];
    }
    buffer[j] = '#';
    j++;
    for (int i = 0; i < HUMEDAD - 1; i++, j++) {
        buffer[j] = datos->humedad[i];
    }
    buffer[j] = '#';
    j++;
    for (int i = 0; i < PRESSION; i++, j++) {
        buffer[j] = datos->presionAtmosferica[i];
    }
    buffer[j] = '#';
    j++;
    for (int i = 0; i < PRECIPITACION; i++, j++) {
        buffer[j] = datos->precipitacion[i];
    }

    //Enviamos la trama
    write(STDOUT_FILENO, "\nEnviando datos...\n", sizeof("\nEnviando datos...\n"));
    write(*sockfd, buffer, TRAMA);

    //Respuesta de Jack
    read(*sockfd, buffer, TRAMA);
    for (int j = 15; buffer[j] != '\0'; j++) {
        write(STDOUT_FILENO, &buffer[j], sizeof(char));
    }
    write(STDOUT_FILENO, "\n\n", sizeof("\n\n"));
}

/*
* Envia la trama de desconexio al servidor indicat per el socket
*/
void enviarTramaDesconec(int *sockfd, char *nombre) {
    int j;
    char danny[6], buffer[TRAMA];

    strcpy(danny, "DANNY");
    for (int i = 0; i < (int) strlen(danny); i++) {
        buffer[i] = danny[i];
    }
    for (int i = (int) strlen(danny); i < 14; i++) {
        buffer[i] = '\0';
    }
    buffer[14] = 'Q';
    j = 15;
    for (int i = 0; i < (int) strlen(nombre); i++, j++) {
        buffer[j] = nombre[i];
    }
    for (int i = (int) strlen(nombre) + 14; i < TRAMA; i++) {
        buffer[i] = '\0';
    }
    write(*sockfd, buffer, TRAMA);
}