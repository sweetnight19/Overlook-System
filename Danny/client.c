#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

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
