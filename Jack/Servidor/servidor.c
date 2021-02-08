/**
 * @authors: David Marquet, Joan Casals
 */

#include "servidor.h"

Reg_estacions *trama_estacio;
semaphore *sem_write, *sem_read;

int numClientes, newsock[NUM_CLIENTES], *cerrarThread, sockfd;
pthread_t threadClientes[NUM_CLIENTES];

/*
* Procesa les dades de la trama i escriu en la memoria compartida
*/
void processarLloyd(char buffer[TRAMA], char nom_estacio[NOMBRE])
{
    int i;
    char temperatura[TEMPERATURA], humitat[HUMEDAD], pressio_atmosferica[PRESSION], precipitacio[PRECIPITACION];

    i = 15;
    for (int j = 0; j < FECHA; i++, j++)
    {
    }
    i++;
    i++;
    for (int j = 0; j < HORA; i++, j++)
    {
    }
    i++;
    i++;
    for (int j = 0; j < TEMPERATURA - 1; i++, j++)
    {
        temperatura[j] = buffer[i];
    }
    i++;
    i++;
    for (int j = 0; j < HUMEDAD - 1; i++, j++)
    {
        humitat[j] = buffer[i];
    }
    i++;
    i++;
    for (int j = 0; j < PRESSION; i++, j++)
    {
        pressio_atmosferica[j] = buffer[i];
    }
    i++;
    i++;
    for (int j = 0; j < PRECIPITACION; i++, j++)
    {
        precipitacio[j] = buffer[i];
    }

    //ENVIAR LES DADES A LLOYD
    SEM_wait(sem_write);
    strcpy(trama_estacio->nom_estacio, nom_estacio);
    trama_estacio->humitat = atof(humitat);
    //printf("atof(humitat):%f.2\n", atof(humitat));
    trama_estacio->temperatura = atof(temperatura);
    //printf("atof(temperatura):%f.2\n", atof(temperatura));
    trama_estacio->precipitacio = atof(precipitacio);
    //printf("atof(precipitacio):%f.2\n", atof(precipitacio));
    trama_estacio->pressio_atmos = atof(pressio_atmosferica);
    //printf("atof(pressio_atm):%f.2\n", atof(pressio_atmosferica));
    SEM_signal(sem_read);
}

/*
* Thread que es crea en la nova connexio d'un client, gestiona les trames
*/
void *TareasServidor(void *socket_desc)
{
    //Get the socket descriptor
    int *newsock = (int *)socket_desc;
    int enviat = 0, int_error = 0;
    int i;
    char buffer[TRAMA], buffer2[TRAMA], jack[5], origen[ORIGEN], conexionOK[12], conexionKO[12], dadesOK[9], dadesKO[9], error[15], nom_estacio[ORIGEN];

    strcpy(jack, "JACK");
    strcpy(conexionOK, "CONNEXIO OK");
    strcpy(conexionKO, "ERROR");
    strcpy(dadesOK, "DADES OK");
    strcpy(dadesKO, "DADES KO");
    strcpy(error, "ERROR DE TRAMA");

    read(*newsock, buffer, TRAMA);
    while (buffer[14] != 'Q' && *cerrarThread == EXIT_SUCCESS)
    {
        //Copiamos el "DANNY"
        for (int i = 0; (i < ORIGEN) && (buffer[i - 1] != 'Y'); ++i)
        {
            origen[i] = buffer[i];
        }
        //Ponemos "JACK" para la salida
        for (int i = 0; i < (int)strlen(jack); i++)
        {
            buffer2[i] = jack[i];
        }
        //Llenamos del '\0' la respuesta hasta el tipo
        for (int i = 1 + (int)strlen(jack); i < 14; i++)
        {
            buffer2[i] = '\0';
        }

        //Responder a la trama de conexion
        if (strcmp(origen, "DANNY") == 0 && buffer[14] == 'C')
        {
            //Declarar cadena de tamany TRAMA per copiar desde la posicio 15 buffer[i] != '\0' i copiar les dades per passar a Lloyd
            for (int j = 0, i = 15; buffer[i] != '\0'; j++, i++)
            {
                nom_estacio[j] = buffer[i];
            }
            write(STDOUT_FILENO, "Nueva connexion: ", sizeof("Nueva connexion: "));
            write(STDOUT_FILENO, nom_estacio, strlen(nom_estacio));
            write(STDOUT_FILENO, "\n", sizeof("\n"));

            write(STDOUT_FILENO, "Enviando trama de connexion a ", sizeof("Enviando trama de connexion a "));
            write(STDOUT_FILENO, nom_estacio, strlen(nom_estacio));
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
                write(STDOUT_FILENO, nom_estacio, strlen(nom_estacio));
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

        //Responder a la trama de datos
        if (strcmp(origen, "DANNY") == 0 && buffer[14] == 'D')
        {
            write(STDOUT_FILENO, "Recibiendo trama de datos a ", sizeof("Recibiendo trama de datos a "));
            write(STDOUT_FILENO, nom_estacio, strlen(nom_estacio));
            write(STDOUT_FILENO, "\n", sizeof("\n"));

            //PROCESSAMENT DE LES DADES A ENVIAR A LLOYD
            processarLloyd(buffer, nom_estacio);

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
            if (buffer[14] == 'D' && int_error == 1)
            {
                write(STDOUT_FILENO, "Enviando trama de datos erronea a ",
                      sizeof("Enviando trama de datos erronea a "));
                write(STDOUT_FILENO, nom_estacio, strlen(nom_estacio));
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
            write(STDOUT_FILENO, nom_estacio, strlen(nom_estacio));
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
        if (enviat > -1)
        {
            write(*newsock, buffer2, TRAMA);
            read(*newsock, buffer, TRAMA);
        }
        //Trama de desconnexion
        if (buffer[14] == 'Q')
        {
            write(STDOUT_FILENO, "Recibiendo trama de desconexion de ",
                  sizeof("Recibiendo trama de desconexion de "));
            write(STDOUT_FILENO, nom_estacio, sizeof(strlen(nom_estacio)));
            write(STDOUT_FILENO, "\n", sizeof("\n"));
        }
    }
    return NULL;
}

/*
* Configuracio del servidor Jack
*/
void configurarServidor(int portJack, int *cerrar, Reg_estacions *reg_estacions, semaphore *sem_read_1, semaphore *sem_write_1)
{
    uint16_t port;
    struct sockaddr_in s_addr;

    port = portJack;
    numClientes = EXIT_SUCCESS;
    cerrarThread = cerrar;
    trama_estacio = reg_estacions;
    sem_read = sem_read_1;
    sem_write = sem_write_1;

    SEM_signal(sem_write);

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

    while (*cerrarThread == EXIT_SUCCESS)
    {

        newsock[numClientes] = accept(sockfd, (void *)&c_addr, &c_len);
        if (newsock[numClientes] > 0)
        {
            pthread_create(&threadClientes[numClientes], NULL, TareasServidor, (void *)&newsock[numClientes]);
            numClientes++;
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