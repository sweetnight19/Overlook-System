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
    int j, fd;
    char buffer[TRAMA], danny[6], lectura[100];

    strcpy(danny, "DANNY");
    danny[strlen(danny)] = '\0';

    for (int j = 0; j < (int)strlen("DANNY"); j++)
    {
        buffer[j] = danny[j];
    }
    for (int j = (int)strlen(buffer); j < 14; j++)
    {
        buffer[j] = '\0';
    }

    buffer[14] = 'I';
    j = 15;
    for (int i = 0; i < (int)strlen(datos->imagenes.fotos[0].nomFoto); i++, j++)
    {
        buffer[j] = datos->imagenes.fotos[0].nomFoto[i];
    }
    j++;
    buffer[j] = '#';
    j++;
    for (int i = 0; i < (int)strlen(datos->imagenes.fotos[0].mida); i++, j++)
    {
        buffer[j] = datos->imagenes.fotos[0].mida[i];
    }
    j++;
    buffer[j] = '#';
    j++;
    for (int i = 0; i < MD5SUM; i++, j++)
    {
        buffer[j] = datos->imagenes.fotos[0].md5sum[i];
    }
    j++;
    while (j < TRAMA)
    {
        buffer[j] = '\0';
        j++;
    }
    write(*sockfd, buffer, TRAMA);
    printf("Enviado trama de informacion\n");
    usleep(500);
    buffer[14] = 'F';
    fd = open(datos->imagenes.fotos[0].path, O_RDONLY);
    for (int byte = 0, k = 0; byte < atoi(datos->imagenes.fotos[0].mida); k++)
    {
        byte += read(fd, lectura, 100);
        j = 15;
        for (int i = 0; i < 100; i++, j++)
        {
            buffer[j] = lectura[i];
        }
        write(*sockfd, buffer, TRAMA);
        usleep(500);
        printf("byte= %d\n", byte);
    }
    close(fd);
}

void enviarDatosJack(Datos *datos, int *sockfd)
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

void enviarTramaDesconec(int *sockfd, char *nombre)
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
    buffer[14] = 'Q';
    j = 15;
    for (int i = 0; i < (int)strlen(nombre); i++, j++)
    {
        buffer[j] = nombre[i];
    }
    for (int i = (int)strlen(nombre) + 14; i < TRAMA; i++)
    {
        buffer[i] = '\0';
    }
    write(*sockfd, buffer, TRAMA);
}