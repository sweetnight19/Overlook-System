/**
 * @authors: David Marquet, Joan Casals
 */
#include "client.h"

int configurarCliente(char IPJack[IP], int portJack, int *sockfd, char *nombre)
{
    int i;
    struct sockaddr_in servaddr;
    char danny[6], buffer[TRAMA];

    strcpy(danny, "DANNY");
    danny[strlen(danny)] = '\0';

    // socket create and varification
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd == -1)
    {
        return EXIT_FAILURE;
    }
    else
    {
        bzero(&servaddr, sizeof(servaddr));

        // assign IP, PORT
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr(IPJack);
        servaddr.sin_port = htons(portJack);

        // connect the client socket to server socket
        if (connect(*sockfd, (void *)&servaddr, sizeof(servaddr)) != 0)
        {
            return EXIT_FAILURE;
        }
        else
        {
            for (int j = 0; j < (int)strlen("DANNY"); j++)
            {
                buffer[j] = danny[j];
            }
            for (int j = (int)strlen(buffer); j < 14; j++)
            {
                buffer[j] = '\0';
            }

            buffer[14] = 'C';
            i = 15;
            for (int j = 0; i < TRAMA && j < (int)strlen(nombre); i++, j++)
            {
                buffer[i] = nombre[j];
            }
            for (int j = i; j < TRAMA; j++)
            {
                buffer[j] = '\0';
            }

            //Enviamos trama de conexion
            write(*sockfd, buffer, TRAMA);
            read(*sockfd, buffer, TRAMA);
            for (int j = 15; buffer[j] != '\0'; j++)
            {
                write(STDOUT_FILENO, &buffer[j], sizeof(char));
            }
            write(STDOUT_FILENO, "\n\n", sizeof("\n\n"));
        }
        return EXIT_SUCCESS;
    }
}

void enviarDatosWendy(Datos *datos, int *sockfd)
{
    char buffer[TRAMA], origen[6];

    strcpy(origen, "DANNY");
    for (int i = 0; i < (int)strlen(origen); i++)
    {
        buffer[i] = origen[i];
    }
    for (int i = strlen(buffer); i < 14; i++)
    {
        buffer[i] = '\0';
    }
    buffer[14] = 'I';
    printf("nomFoto: %s\n", datos->imagenes.fotos[0].nomFoto);
    strcat(buffer, datos->imagenes.fotos[0].nomFoto);
    strcat(buffer, "#");
    printf("mida: %s\n", datos->imagenes.fotos[0].mida);
    strcat(buffer, datos->imagenes.fotos[0].mida);
    strcat(buffer, "#");
    printf("md5sum: %s\n", datos->imagenes.fotos[0].md5sum);
    strcat(buffer, datos->imagenes.fotos[0].md5sum);
    for (int i = strlen(buffer); i < TRAMA; i++)
    {
        origen[i] = '\0';
    }
    printf("trama previa de enviar la foto: %s\n", buffer);
    //write(STDOUT_FILENO, buffer, sizeof(char) * TRAMA);
    write(*sockfd, buffer, sizeof(char) * TRAMA);
}
void enviarDatos(Datos *datos, int *sockfd)
{
    int j;
    char danny[6], buffer[TRAMA];

    strcpy(danny, "DANNY");
    for (int i = 0; i < (int)strlen(danny); i++)
    {
        buffer[i] = danny[i];
    }
    for (int i = (int)strlen(danny); i < 14; i++)
    {
        buffer[i] = '\0';
    }
    buffer[14] = 'D';
    j = 15;
    for (int i = 0; i < (int)strlen(datos->fecha); i++, j++)
    {
        buffer[j] = datos->fecha[i];
    }
    buffer[j] = '#';
    for (int i = 0; i < (int)strlen(datos->hora); i++, j++)
    {
        buffer[j] = datos->hora[i];
    }
    buffer[j] = '#';
    for (int i = 0; i < (int)strlen(datos->temperatura); i++, j++)
    {
        buffer[j] = datos->temperatura[i];
    }
    buffer[j] = '#';
    for (int i = 0; i < (int)strlen(datos->humedad); i++, j++)
    {
        buffer[j] = datos->humedad[i];
    }
    buffer[j] = '#';
    for (int i = 0; i < (int)strlen(datos->presionAtmosferica); i++, j++)
    {
        buffer[j] = datos->presionAtmosferica[i];
    }
    buffer[j] = '#';
    for (int i = 0; i < (int)strlen(datos->precipitacion); i++, j++)
    {
        buffer[j] = datos->precipitacion[i];
    }
    write(STDOUT_FILENO, "\nEnviando datos...\n", sizeof("\nEnviando datos...\n"));
    write(*sockfd, buffer, TRAMA);
    read(*sockfd, buffer, TRAMA);
    for (int j = 15; buffer[j] != '\0'; j++)
    {
        write(STDOUT_FILENO, &buffer[j], sizeof(char));
    }
    write(STDOUT_FILENO, "\n\n", sizeof("\n\n"));
}