/**
 * @authors: David Marquet, Joan Casals
 */

#include "servidor.h"

int cerrarThread;

void *TareasServidor(void *socket_desc)
{
    //Get the socket descriptor
    int *newsock = (int *)socket_desc;

    int enviat = 0;
    int i, j, byte;
    char buffer[TRAMA], buffer2[TRAMA], wendy[6],
        origen[ORIGEN], conexionOK[12], conexionKO[12],
        dadesOK[9], dadesKO[9], error[15],
        nomFitxer[NAMEFILE], mida[MD5SUM], /*checksum[MD5SUM],*/ nomEstacio[TRAMA];

    strcpy(wendy, "WENDY");
    strcpy(conexionOK, "CONNEXIO OK");
    strcpy(conexionKO, "ERROR");
    strcpy(dadesOK, "DADES OK");
    strcpy(dadesKO, "DADES KO");
    strcpy(error, "ERROR DE TRAMA");

    //sprintf(buffer, "Nueva conexion desde %s:%hu\n", inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));
    //write(STDOUT_FILENO, buffer, sizeof(char) * strlen(buffer));
    read(*newsock, buffer, TRAMA);
    while (buffer[14] != 'Q' && cerrarThread == EXIT_SUCCESS)
    {
        for (int i = 0; i < ORIGEN; ++i)
        {
            origen[i] = buffer[i];
        }
        for (int i = 0; i < (int)strlen(wendy); i++)
        {
            buffer2[i] = wendy[i];
        }
        for (int i = 1 + (int)strlen(wendy); i < 14; i++)
        {
            buffer2[i] = '\0';
        }

        //Responder a la trama de conexion
        if (strcmp(origen, "DANNY") == 0 && buffer[14] == 'C')
        {

            for (j = 0, i = 15; buffer[i] != '\0'; i++)
            {
                nomEstacio[j] = buffer[i];
            }
            nomEstacio[j] = '\0';
            //sprintf(buffer2, "Nueva connexion: %s", nomEstacio);
            write(STDOUT_FILENO, "Nueva connexion: ", sizeof("Nueva connexion: "));
            write(STDOUT_FILENO, nomEstacio, sizeof(char) * strlen(nomEstacio));
            for (int count = 0; count < atoi(mida); i++)
            {
                count += read(*newsock, buffer, TRAMA);
            }

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

        //Responder a la trama de imagenes
        if (strcmp(origen, "DANNY") == 0 && buffer[14] == 'I')
        {
            i = 15;
            for (j = 0; buffer[i] != '#'; i++, j++)
            {
                nomFitxer[j] = buffer[i];
            }
            nomFitxer[j] = '\0';
            i++;
            for (j = 0; buffer[i] != '#'; i++, j++)
            {
                mida[j] = buffer[i];
            }
            mida[j] = '\0';
            i++;
            for (j = 0; buffer[i] != '\0'; i++, j++)
            {
                //checksum[j] = buffer[i];
            }
            //checksum[j] = '\0';
            write(STDOUT_FILENO, nomFitxer, sizeof(char) * strlen(nomFitxer));

            /*
            write(STDOUT_FILENO, "Enviando trama inicial correcta de imagen\n", sizeof("Enviando trama inicial correcta de imagen\n"));
            buffer2[14] = 'B';
            //i = 15;
            for (int j = 0; i < TRAMA && j < (int)strlen(dadesOK); i++, j++)
            {
                buffer2[i] = dadesOK[j];
            }
            for (int j = i; j < TRAMA; j++)
            {
                buffer2[j] = '\0';
            }
            */

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
        write(*newsock, buffer2, TRAMA);
        byte = read(*newsock, buffer, TRAMA);

        if (cerrarThread == EXIT_SUCCESS && byte > 0)
        {
            enviat = 0;
            //Al estar bloqueado en read, el ctrl+c no llega a cerrar el thread
            //read(*newsock, buffer, TRAMA);
        }

        //enviat = 0;
    }

    return NULL;
}

void signalHandler()
{
    write(STDOUT_FILENO, "\nDisconnecting Jack...\n", sizeof("\nDisconnecting Jack...\n"));
    cerrarThread = EXIT_FAILURE;
    raise(SIGINT);
}

void configurarServidor(int portJack)
{
    int numClientes, newsock[NUM_CLIENTES], sockfd;
    pthread_t threadClientes[NUM_CLIENTES];
    uint16_t port;
    struct sockaddr_in s_addr;

    signal(SIGINT, signalHandler);
    port = portJack;
    numClientes = EXIT_SUCCESS;
    cerrarThread = EXIT_SUCCESS;

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

    while (cerrarThread == EXIT_SUCCESS)
    {
        struct sockaddr_in c_addr;
        socklen_t c_len = sizeof(c_addr);

        newsock[numClientes] = accept(sockfd, (void *)&c_addr, &c_len);
        if (newsock[numClientes] < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        pthread_create(&threadClientes[numClientes], NULL, TareasServidor, (void *)&newsock[numClientes]);
        numClientes++;
    }

    for (int i = 0; i < numClientes; i++)
    {
        pthread_cancel(threadClientes[i]);
    }

    for (int i = 0; i < numClientes; i++)
    {
        pthread_join(threadClientes[i], NULL);
    }
    for (int i = 0; i < numClientes; i++)
    {
        close(newsock[i]);
    }
    close(sockfd);
}