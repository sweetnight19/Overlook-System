#include "client.h"

/*
int
main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Error: missing arguments\n");
        exit(EXIT_FAILURE);
    }

    uint16_t port;
    int aux = atoi(argv[2]);
    if (aux < 1 || aux > 65535) {
        fprintf(stderr, "Error: %s: invalid port\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    port = aux;

    struct sockaddr_in s_addr;
    if (inet_pton(AF_INET,argv[1], &(s_addr.sin_addr)) == 0) {
        fprintf(stderr, "inet_aton (%s): %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }

    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        perror("socket TCP");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in s_addr;
    bzero(&s_addr, sizeof(&s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(port);
    s_addr.sin_addr = ip_addr;

    if (connect(sockfd, (void *) &s_addr, sizeof(s_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    char msg[] = "Hola!";
    write(sockfd, msg, sizeof(msg));

    return EXIT_SUCCESS;
}
*/
void configurarCliente(char IPJack[IP], int portJack, int *sockfd, char *nombre) {
    int i;
    struct sockaddr_in servaddr;
    char danny[6] = "DANNY", buffer[TRAMA];

    // socket create and varification
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd == -1) {
        printf("socket creation failed...\n");
    } else {
        //printf("Socket successfully created..\n");
        bzero(&servaddr, sizeof(servaddr));

        // assign IP, PORT
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr(IPJack);
        servaddr.sin_port = htons(portJack);

        // connect the client socket to server socket
        if (connect(*sockfd, (void *) &servaddr, sizeof(servaddr)) != 0) {
            printf("connection with the server failed...\n");
        } else {
            for (int i = 0; i < strlen(danny - 1); i++) {
                buffer[i] = danny[i];
            }
            for (int i = 1 + strlen(buffer); i < 14; i++) {
                buffer[i] = '\0';
            }

            buffer[14] = 'C';
            i = 15;
            for (int j = 0; i < TRAMA && j < strlen(nombre); i++, j++) {
                buffer[i] = nombre[j];
            }
            for (int j = i; j < TRAMA; j++) {
                buffer[j] = '\0';
            }
            write(*sockfd, buffer, TRAMA);
            read(*sockfd, buffer, TRAMA);
            for (int j = 15; buffer[j] != '\0'; j++) {
                printf("%c", buffer[j]);
            }
            printf("\n");
        }
    }
}

void enviarDatos(Datos *datos, int *sockfd) {
    int j;
    char danny[6], buffer[TRAMA];

    strcpy(danny, "DANNY");
    for (int i = 0; i < strlen(danny); i++) {
        buffer[i] = danny[i];
    }
    for (int i = strlen(danny); i < 14; i++) {
        buffer[i] = '\0';
    }
    buffer[14] = 'D';
    j = 15;
    for (int i = 0; i < strlen(datos->fecha); i++, j++) {
        buffer[j] = datos->fecha[i];
    }
    buffer[j] = '#';
    for (int i = 0; i < strlen(datos->hora); i++, j++) {
        buffer[j] = datos->hora[i];
    }
    buffer[j] = '#';
    for (int i = 0; i < strlen(datos->temperatura); i++, j++) {
        buffer[j] = datos->temperatura[i];
    }
    buffer[j] = '#';
    for (int i = 0; i < strlen(datos->humedad); i++, j++) {
        buffer[j] = datos->humedad[i];
    }
    buffer[j] = '#';
    for (int i = 0; i < strlen(datos->presionAtmosferica); i++, j++) {
        buffer[j] = datos->presionAtmosferica[i];
    }
    buffer[j] = '#';
    for (int i = 0; i < strlen(datos->precipitacion); i++, j++) {
        buffer[j] = datos->precipitacion[i];
    }
    printf("enviant dades...\n");
    write(*sockfd, buffer, TRAMA);
    read(*sockfd, buffer, TRAMA);
    for (int j = 15; buffer[j] != '\0'; j++) {
        printf("%c", buffer[j]);
    }
    printf("\n");
}