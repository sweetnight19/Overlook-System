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
    int i, j, fd;
    char buffer[TRAMA], buffer2[TRAMA], wendy[6],
        origen[ORIGEN], conexionOK[12], conexionKO[12],
        dadesOK[9], dadesKO[9], error[15],
        nomFitxer[NAMEFILE], mida[MD5SUM],
        checksum[MD5SUM], nomEstacio[NOMBRE], *foto;

    strcpy(wendy, "WENDY");
    strcpy(conexionOK, "CONNEXIO OK");
    strcpy(conexionKO, "ERROR");
    strcpy(dadesOK, "DADES OK");
    strcpy(dadesKO, "DADES KO");
    strcpy(error, "ERROR DE TRAMA");

    read(*newsock, buffer, TRAMA);
    while (buffer[14] != 'Q' && cerrarThread == EXIT_SUCCESS)
    {
        //Copiamos el "DANNY"
        for (int i = 0; (i < ORIGEN) && (buffer[i - 1] != 'Y'); ++i)
        {
            origen[i] = buffer[i];
        }

        //Ponemos "WENDY" para la salida
        for (int i = 0; i < (int)strlen(wendy); i++)
        {
            buffer2[i] = wendy[i];
        }

        //Llenamos del '\0' la respuesta hasta el tipo
        for (int i = 1 + (int)strlen(wendy); i < 14; i++)
        {
            buffer2[i] = '\0';
        }

        //Responder a la trama de conexion
        if (strcmp(origen, "DANNY") == 0 && buffer[14] == 'C')
        {

            for (j = 0, i = 15; buffer[i] != '\0'; i++, j++)
            {
                nomEstacio[j] = buffer[i];
            }
            nomEstacio[strlen(nomEstacio)] = '\0';
            write(STDOUT_FILENO, "Nueva connexion: ", sizeof("Nueva connexion: "));
            write(STDOUT_FILENO, nomEstacio, sizeof(char) * strlen(nomEstacio));
            write(STDOUT_FILENO, "\n", sizeof("\n"));
            for (int count = 0; count < atoi(mida); i++)
            {
                count += read(*newsock, buffer, TRAMA);
            }

            write(STDOUT_FILENO, "Enviando trama de connexion a ", sizeof("Enviando trama de connexion a "));
            write(STDOUT_FILENO, nomEstacio, sizeof(strlen(nomEstacio)));
            write(STDOUT_FILENO, "\n", sizeof("\n"));
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
                write(STDOUT_FILENO, "Enviando trama de connexion erronea a ",
                      sizeof("Enviando trama de connexion erronea a "));
                write(STDOUT_FILENO, nomEstacio, sizeof(strlen(nomEstacio)));
                write(STDOUT_FILENO, "\n", sizeof("\n"));
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
            write(STDOUT_FILENO, "Recibiendo trama de imagen de ",
                  sizeof("Recibiendo trama de imagen de "));
            write(STDOUT_FILENO, nomEstacio, sizeof(strlen(nomEstacio)));
            write(STDOUT_FILENO, "\n", sizeof("\n"));
            i = 15;
            for (j = 0; buffer[i] != '#'; i++, j++)
            {
                nomFitxer[j] = buffer[i];
            }
            nomFitxer[j] = '\0';
            printf("nomFitxer: %s\n", nomFitxer);
            i++;
            for (j = 0; buffer[i] != '#'; i++, j++)
            {
                mida[j] = buffer[i];
            }
            mida[j] = '\0';
            printf("mida: %s\n", mida);
            i++;
            for (j = 0; buffer[i] != '\0'; i++, j++)
            {
                checksum[j] = buffer[i];
            }
            checksum[j] = '\0';
            printf("checksum: %s\n", checksum);
            printf("-----------------------\n");
            foto = (char *)realloc(foto, sizeof(char) * atoi(mida));
            j = 0;
            for (int byte = 0; byte < atoi(mida); i++)
            {
                byte += read(*newsock, buffer, TRAMA);
                byte -= 15;
                for (int i = 15; i < TRAMA; i++, j++)
                {
                    foto[j] = buffer[i];
                }
                usleep(500);
                printf("%d%%\n", byte * 100 / atoi(mida));
            }
            fd = open(nomFitxer, O_CREAT | O_WRONLY, 0666);
            write(fd, foto, atoi(mida));
            printf("foto creada\n");
            close(fd);
            free(foto);
            enviat = 2;
        }
        else
        {
            if (buffer[14] == 'D')
            {
                write(STDOUT_FILENO, "Enviando trama de datos erronea a ",
                      sizeof("Enviando trama de datos erronea a "));
                write(STDOUT_FILENO, nomEstacio, sizeof(strlen(nomEstacio)));
                write(STDOUT_FILENO, "\n", sizeof("\n"));
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
            write(STDOUT_FILENO, "Enviando trama erronea a ", sizeof("Enviando trama erronea a "));
            write(STDOUT_FILENO, nomEstacio, sizeof(strlen(nomEstacio)));
            write(STDOUT_FILENO, "\n", sizeof("\n"));
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
        if (enviat == 1)
        {
            write(*newsock, buffer2, TRAMA);
            read(*newsock, buffer, TRAMA);
        }
        if (enviat == 2)
        {
            read(*newsock, buffer, TRAMA);
        }
        //Trama de desconnexion
        if (buffer[14] == 'Q')
        {
            write(STDOUT_FILENO, "Recibiendo trama de desconexion de ",
                  sizeof("Recibiendo trama de desconexion de "));
            write(STDOUT_FILENO, nomEstacio, sizeof(strlen(nomEstacio)));
            write(STDOUT_FILENO, "\n", sizeof("\n"));
        }
    }
    return NULL;
}

void signalHandler()
{
    write(STDOUT_FILENO, "\nDisconnecting Wendy...\n", sizeof("\nDisconnecting Wendy...\n"));
    cerrarThread = EXIT_FAILURE;
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

    sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
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
    struct sockaddr_in c_addr;
    socklen_t c_len = sizeof(c_addr);

    while (cerrarThread == EXIT_SUCCESS)
    {

        newsock[numClientes] = accept(sockfd, (void *)&c_addr, &c_len);
        if (newsock[numClientes] > 0)
        {
            pthread_create(&threadClientes[numClientes], NULL, TareasServidor, (void *)&newsock[numClientes]);
            numClientes++;
            //perror("accept");
            //exit(EXIT_FAILURE);
        }
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