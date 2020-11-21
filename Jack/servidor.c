/**
 * @authors: David Marquet, Joan Casals
 */

#include "servidor.h"

void configurarServidor(int portJack) {
    uint16_t port;
    int sockfd;
    struct sockaddr_in s_addr;

    port = portJack;
    printf("Port: %d\n",portJack);
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
        //ssize_t len;
        close(newsock);
    }
}