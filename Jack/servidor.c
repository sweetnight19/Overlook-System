/**
 * @authors: David Marquet, Joan Casals
 */

#include "servidor.h"

void configurarServidor(int portJack)
{
    uint16_t port;
    int sockfd, i, newsock, enviat;
    struct sockaddr_in s_addr;
    char buffer[TRAMA], buffer2[TRAMA], jack[5], origen[ORIGEN], conexionOK[12], conexionKO[12], dadesOK[9], dadesKO[9], error[15];

    strcpy(jack, "JACK");
    strcpy(conexionOK, "CONNEXIO OK");
    strcpy(conexionKO, "ERROR");
    strcpy(dadesOK, "DADES OK");
    strcpy(dadesKO, "DADES KO");
    strcpy(error, "ERROR DE TRAMA");
    port = portJack;
    enviat = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        perror("socket TCP");
        exit(EXIT_FAILURE);
    }

    bzero(&s_addr, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(port);
    s_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (void *)&s_addr, sizeof(s_addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    listen(sockfd, NUM_CLIENTES);

    while (1)
    {
        struct sockaddr_in c_addr;
        socklen_t c_len = sizeof(c_addr);

        newsock = accept(sockfd, (void *)&c_addr, &c_len);
        if (newsock < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        sprintf(buffer, "Nueva conexion desde %s:%hu\n", inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));
        write(STDOUT_FILENO, buffer, sizeof(char) * strlen(buffer));
        read(newsock, buffer, TRAMA);
        while (buffer[14] != 'Q')
        {
            for (int i = 0; i < ORIGEN; ++i)
            {
                origen[i] = buffer[i];
            }
            for (int i = 0; i < (int)strlen(jack); i++)
            {
                buffer2[i] = jack[i];
            }
            for (int i = 1 + (int)strlen(jack); i < 14; i++)
            {
                buffer2[i] = '\0';
            }

            //Responder a la trama de conexion
            if (strcmp(origen, "DANNY") == 0 && buffer[14] == 'C')
            {
                write(STDOUT_FILENO, "Enviando trama de connexion\n", sizeof("Enviando trama de connexion\n"));
                buffer2[14] = 'O';
                i = 15;
                for (int j = 0; i < TRAMA && j < (int)strlen(conexionOK); i++, j++)
                {
                    buffer2[i] = conexionOK[j];
                }
                for (int j = i; j < TRAMA; j++)
                {
                    buffer2[j] = '\0';
                }
                enviat = 1;
            }
            else
            {
                if (buffer[14] == 'C')
                {
                    write(STDOUT_FILENO, "Enviando trama de connexion erronea\n",
                          sizeof("Enviando trama de connexion erronea\n"));
                    buffer2[14] = 'E';
                    i = 15;
                    for (int j = 0; i < TRAMA && j < (int)strlen(conexionKO); i++, j++)
                    {
                        buffer2[i] = conexionKO[j];
                    }
                    for (int j = i; j < TRAMA; j++)
                    {
                        buffer2[j] = '\0';
                    }
                    enviat = 1;
                }
            }

            //Responder a la trama de datos
            if (strcmp(origen, "DANNY") == 0 && buffer[14] == 'D')
            {
                write(STDOUT_FILENO, "Enviando trama de datos\n", sizeof("Enviando trama de datos\n"));
                buffer2[14] = 'B';
                i = 15;
                for (int j = 0; i < TRAMA && j < (int)strlen(dadesOK); i++, j++)
                {
                    buffer2[i] = dadesOK[j];
                }
                for (int j = i; j < TRAMA; j++)
                {
                    buffer2[j] = '\0';
                }
                enviat = 1;
            }
            else
            {
                if (buffer[14] == 'D')
                {
                    write(STDOUT_FILENO, "Enviando trama de datos erronea\n",
                          sizeof("Enviando trama de datos erronea\n"));
                    buffer2[14] = 'K';
                    i = 15;
                    for (int j = 0; i < TRAMA && j < (int)strlen(dadesKO); i++, j++)
                    {
                        buffer2[i] = dadesKO[j];
                    }
                    for (int j = i; j < TRAMA; j++)
                    {
                        buffer2[j] = '\0';
                    }
                    enviat = 1;
                }
            }

            //Responder a la trama erronea
            if (enviat == 0)
            {
                write(STDOUT_FILENO, "Enviando trama erronea\n", sizeof("Enviando trama erronea\n"));
                buffer2[14] = 'Z';
                i = 15;
                for (int j = 0; i < TRAMA && j < (int)strlen(error); i++, j++)
                {
                    buffer2[i] = error[j];
                }
                for (int j = i; j < TRAMA; j++)
                {
                    buffer2[j] = '\0';
                }
            }
            write(newsock, buffer2, TRAMA);
            read(newsock, buffer, TRAMA);

            enviat = 0;
        }
    }
}