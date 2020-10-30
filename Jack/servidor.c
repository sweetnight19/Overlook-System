#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Error: missing port\n");
        exit(EXIT_FAILURE);
    }

    uint16_t port;
    int aux = atoi(argv[1]);
    if (aux < 1 || aux > 65535) {
        fprintf(stderr, "Error: %s: invalid port\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    port = aux;

    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        perror("socket TCP");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in s_addr;
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
        printf("New connection from %s:%hu\n",
               inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));

        ssize_t len;
        char buff[513];
        do {
            len = read(newsock, buff, 512);

            buff[len] = 0;
            printf("%s\n", buff);
        } while (len > 0);
        close(newsock);
    }

    return EXIT_SUCCESS;
}