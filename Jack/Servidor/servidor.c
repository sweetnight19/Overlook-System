/**
 * @authors: David Marquet, Joan Casals
 */

#include "servidor.h"

<<<<<<< HEAD
Reg_estacions *trama_estacio;

=======
>>>>>>> f7be03f801cc6c5977f86f8c26faeafaeda6df89
int numClientes, newsock[NUM_CLIENTES], cerrarThread, sockfd;
pthread_t threadClientes[NUM_CLIENTES];

void *TareasServidor(void *socket_desc)
{
    //Get the socket descriptor
    int *newsock = (int *)socket_desc;
    int enviat = 0;
    int i, byte;
    char buffer[TRAMA], buffer2[TRAMA], jack[5], origen[ORIGEN], conexionOK[12], conexionKO[12], dadesOK[9], dadesKO[9], error[15], nom_estacio[TRAMA];

    strcpy(jack, "JACK");
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
<<<<<<< HEAD
            //Declarar cadena de tamany TRAMA per copiar desde la posicio 15 buffer[i] != '\0' i copiar les dades per passar a Lloyd
            for (int j = 0, i = 15; buffer[i] != '\0'; j++, i++)
            {
                nom_estacio[j] = buffer[i];
            }
=======
>>>>>>> f7be03f801cc6c5977f86f8c26faeafaeda6df89

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

                //PROCESSAMENT DE LES DADES A ENVIAR A LLOYD

                int contador = 0;
                char *data, *hora, *temperatura, *humitat, *pressio_atmosferica, *precipitacio;
                bool ok = true;

                while (buffer[contador] != '#')
                {
                    data = buffer[contador];
                    contador++;
                }

                if (strlen(data) > 10)
                {
                    ok = false;
                }

                contador++;

                while (buffer[contador] != '#')
                {
                    hora = buffer[contador];
                    contador++;
                }

                if (strlen(hora) > 8)
                {
                    ok = false;
                }

                contador++;

                while (buffer[contador] != '#')
                {
                    temperatura = buffer[contador];
                    contador++;
                }

                if (strlen(temperatura))
                {
                    ok = false;
                }

                contador++;

                while (buffer[contador] != '#')
                {
                    humitat = buffer[contador];
                    contador++;
                }

                if (strlen(humitat))
                {
                    ok = false;
                }

                contador++;

                while (buffer[contador] != '#')
                {
                    pressio_atmosferica = buffer[contador];
                    contador++;
                }

                if (strlen(pressio_atmosferica))
                {
                    ok = false;
                }

                contador++;

                while (buffer[contador] != '#')
                {
                    precipitacio = buffer[contador];
                    contador++;
                }

                if (strlen(precipitacio))
                {
                    ok = false;
                }

                if (ok == true)
                {
                    //ENVIAR LES DADES A LLOYD
                }
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
    for (int i = 0; i < numClientes; i++)
    {
        close(newsock[i]);
    }
    printf("newsock cerrados\n");
    close(sockfd);
    printf("sockfd cerrado\n");
    for (int i = 0; i < numClientes; i++)
    {
        pthread_join(threadClientes[i], NULL);
    }
    printf("threads cerrados\n");
    raise(SIGINT);
}

void configurarServidor(int portJack)
{
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
    else
    {
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

            newsock[numClientes] = accept(sockfd, (void *)&c_addr, &c_len);
            if (newsock[numClientes] < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            pthread_create(&threadClientes[numClientes], NULL, TareasServidor, (void *)&newsock[numClientes]);
            numClientes++;
        }
    }
}