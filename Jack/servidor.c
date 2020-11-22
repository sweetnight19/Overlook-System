/**
 * @authors: David Marquet, Joan Casals
 */

#include "servidor.h"

void configurarServidor(int portJack) {
    uint16_t port;
    int sockfd, i;
    struct sockaddr_in s_addr;
    char buffer[TRAMA], buffer2[TRAMA], jack[5], origen[ORIGEN], conexionOK[12], conexionKO[12];

    strcpy(jack, "JACK");
    strcpy(conexionOK, "CONNEXIO OK");
    strcpy(conexionKO, "ERROR");
    port = portJack;
    printf("Port: %d\n", portJack);
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        perror("socket TCP");
        exit(EXIT_FAILURE);
    }

    bzero(&s_addr, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(port);
    s_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (void *) &s_addr, sizeof(s_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    listen(sockfd, 5);

    while (1) {
        struct sockaddr_in c_addr;
        socklen_t c_len = sizeof(c_addr);

        int newsock = accept(sockfd, (void *) &c_addr, &c_len);
        if (newsock < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("New connection from %s:%hu\n", inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));
        read(newsock, buffer, TRAMA);
        while (buffer[14] != 'Q') {
            for (int i = 0; i < ORIGEN; ++i) {
                origen[i] = buffer[i];
            }
            for (int i = 0; i < strlen(jack); i++) {
                buffer2[i] = jack[i];
            }
            for (int i = 1 + strlen(jack); i < 14; i++) {
                buffer2[i] = '\0';
            }
            if (strcmp(origen, "DANNY") == 0 && buffer[14] == 'C') {
                buffer2[14] = 'O';
                i = 15;
                for (int j = 0; i < TRAMA && j < strlen(conexionOK); i++, j++) {
                    buffer2[i] = conexionOK[j];
                }
                for (int j = i; j < TRAMA; j++) {
                    buffer2[j] = '\0';
                }
            } else {
                buffer2[14] = 'E';
                i = 15;
                for (int j = 0; i < TRAMA && j < strlen(conexionKO); i++, j++) {
                    buffer2[i] = conexionKO[j];
                }
                for (int j = i; j < TRAMA; j++) {
                    buffer2[j] = '\0';
                }
            }
            write(newsock, buffer2, TRAMA);
            read(newsock, buffer, TRAMA);
            if (buffer[14] == 'Q') {
                write(STDOUT_FILENO, "\nTANCANT SOCKET...\n", sizeof("\nTANCANT SOCKET...\n"));
                close(newsock);
                raise(SIGINT);
            }
        }
    }
}