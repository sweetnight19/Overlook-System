/**
 * @authors: David Marquet, Joan Casals
 */

#include "servidor.h"

int cerrarThread = EXIT_SUCCESS;

/*
*Calcula el checksum de la foto per saber si s'ha rebut correctament
*/
char *calcularMd5sum(char *path) {
    char *md5sum = NULL;
    int fdpipe[2];
    int pid = 0;

    pipe(fdpipe);
    pid = fork();
    md5sum = (char *) malloc(sizeof(char) * MD5SUM);
    if (pid == 0) {
        char *args[] = {"md5sum", path, NULL};
        dup2(fdpipe[1], STDOUT_FILENO);
        close(fdpipe[0]);
        close(fdpipe[1]);
        execvp(args[0], args);
    } else {
        wait(NULL);
        close(fdpipe[1]);
        read(fdpipe[0], md5sum, MD5SUM);
        close(fdpipe[0]);
    }
    return md5sum;
}

int checkMD5(char md5_1[MD5SUM], char md5_2[MD5SUM]) {
    int flag = 0;

    for (int i = 0; i < MD5SUM; i++) {
        if (md5_1[i] == md5_2[i]) {

        } else {
            printf("error: %d '%c' '%c'\n", i, md5_1[i], md5_2[i]);
            flag = 1;
        }
    }
    return flag;
}

/*
* Thread creat a la connexio de un client nou, gestiona les trames i la comunicacio
*/
void *TareasServidor(void *socket_desc) {
    int *newsock = (int *) socket_desc;
    int enviat = 0;
    int i = 0;
    int j = 0;
    int fd = -1;
    int byte = 0;
    char buffer[TRAMA] = "";
    char buffer2[TRAMA] = "";
    char wendy[6] = "";
    char origen[ORIGEN] = "";
    char conexionOK[12] = "";
    char conexionKO[12] = "";
    char dadesOK[9] = "";
    char dadesKO[9] = "";
    char error[15] = "";
    char nomFitxer[NAMEFILE] = "";
    char mida[MD5SUM] = "";
    char checksum[MD5SUM] = "";
    char nomEstacio[NOMBRE] = "";
    char foto[100] = "";
    char path[100] = "";
    char imagenOk[10] = "";
    char imagenKo[10] = "";
    char *md5sum = NULL;

    strcpy(wendy, "WENDY\0");
    strcpy(conexionOK, "CONNEXIO OK\0");
    strcpy(conexionKO, "ERROR\0");
    strcpy(dadesOK, "DADES OK\0");
    strcpy(dadesKO, "DADES KO\0");
    strcpy(error, "ERROR DE TRAMA\0");
    strcpy(imagenKo, "IMATGE KO\0");
    strcpy(imagenOk, "IMATGE OK\0");

    read(*newsock, buffer, TRAMA);
    while (buffer[14] != 'Q' && cerrarThread == EXIT_SUCCESS) {
        //Copiamos el "DANNY"
        for (int i = 0; i < ORIGEN && buffer[i] >= 'A' && buffer[i] <= 'Z'; i++) {
            origen[i] = buffer[i];
        }
        origen[5] = '\0';

        //Ponemos "WENDY" para la salida
        for (int i = 0; i < (int) strlen(wendy); i++) {
            buffer2[i] = wendy[i];
        }

        //Llenamos del '\0' la respuesta hasta el tipo
        for (int i = 1 + (int) strlen(wendy); i < 14; i++) {
            buffer2[i] = '\0';
        }

        //Responder a la trama de conexion
        if (strcmp(origen, "DANNY\0") == 0 && buffer[14] == 'C') {
            for (j = 0, i = 15; buffer[i] != '\0'; i++, j++) {
                nomEstacio[j] = buffer[i];
            }
            nomEstacio[strlen(nomEstacio)] = '\0';
            write(STDOUT_FILENO, "Nueva connexion: \0", sizeof("Nueva connexion: \0"));
            write(STDOUT_FILENO, nomEstacio, strlen(nomEstacio));
            write(STDOUT_FILENO, "\n", sizeof("\n"));
            for (int count = 0; count < atoi(mida); i++) {
                count += read(*newsock, buffer, TRAMA);
            }

            write(STDOUT_FILENO, "Enviando trama de connexion a \0", sizeof("Enviando trama de connexion a \0"));
            write(STDOUT_FILENO, nomEstacio, strlen(nomEstacio));
            write(STDOUT_FILENO, "\n", sizeof("\n"));
            buffer2[14] = 'O';
            i = 15;
            for (int j = 0; i < TRAMA && j < (int) strlen(conexionOK); i++, j++) {
                buffer2[i] = conexionOK[j];
            }
            for (int j = i; j < TRAMA; j++) {
                buffer2[j] = '\0';
            }
            enviat = 1;
        } else {
            if (buffer[14] == 'C') {
                write(STDOUT_FILENO, "Enviando trama de connexion erronea a \0",
                      sizeof("Enviando trama de connexion erronea a \0"));
                write(STDOUT_FILENO, nomEstacio, strlen(nomEstacio));
                write(STDOUT_FILENO, "\n", sizeof("\n"));
                buffer2[14] = 'E';
                i = 15;
                for (int j = 0; i < TRAMA && j < (int) strlen(conexionKO); i++, j++) {
                    buffer2[i] = conexionKO[j];
                }
                for (int j = i; j < TRAMA; j++) {
                    buffer2[j] = '\0';
                }
                enviat = 1;
            }
        }

        //Responder a la trama de imagenes
        if (strcmp(origen, "DANNY\0") == 0 && buffer[14] == 'I') {
            write(STDOUT_FILENO, "-----------------------\n", sizeof("-----------------------\n"));
            write(STDOUT_FILENO, "Recibiendo trama de imagen de \0",
                  sizeof("Recibiendo trama de imagen de \0"));
            write(STDOUT_FILENO, nomEstacio, strlen(nomEstacio));
            write(STDOUT_FILENO, "\n", sizeof("\n"));
            i = 15;
            for (j = 0; buffer[i] != '#'; i++, j++) {
                nomFitxer[j] = buffer[i];
            }
            nomFitxer[j] = '\0';
            write(STDOUT_FILENO, "nombre del fichero: \0", sizeof("nombre del fichero: \0"));
            write(STDOUT_FILENO, nomFitxer, strlen(nomFitxer));
            write(STDOUT_FILENO, "\n", sizeof("\n"));
            i++;
            for (j = 0; buffer[i] != '#' && buffer[i] >= '0' && buffer[i] <= '9'; i++, j++) {
                mida[j] = buffer[i];
            }
            mida[j] = '\0';
            //write(STDOUT_FILENO, "mida: \0", sizeof("mida: \0"));
            //write(STDOUT_FILENO, mida, strlen(mida));
            //write(STDOUT_FILENO, "\n", sizeof("\n"));
            i++;
            i++;
            for (j = 0; buffer[i] != '\0'; i++, j++) {
                if (buffer[i] != '#') {
                    checksum[j] = buffer[i];
                }
            }
            checksum[j] = '\0';
            write(STDOUT_FILENO, "checksum: \0", sizeof("checksum: \0"));
            write(STDOUT_FILENO, checksum, sizeof(char) * MD5SUM);
            write(STDOUT_FILENO, "\n", sizeof("\n"));
            strcpy(path, "Barry/");
            for (int i = 6, j = 0; i < (int) strlen(nomFitxer) + 6; i++, j++) {
                path[i] = nomFitxer[j];
            }
            fd = open(path, O_CREAT | O_WRONLY, 0666);
            byte = 0;
            for (int k = 0; k < (int) strlen(mida); ++k) {
                byte = byte * 10 + mida[k] - '0';
            }
            //byte = atoi(mida);
            for (int k = 0; byte > 100; k++) {
                byte -= 100;
                read(*newsock, buffer, TRAMA);
                for (int i = 15, j = 0; i < TRAMA; i++, j++) {
                    foto[j] = buffer[i];
                }
                write(fd, foto, 100);
            }
            read(*newsock, buffer, TRAMA);
            for (int i = 15, j = 0; i < (byte + 15); i++, j++) {
                foto[j] = buffer[i];
            }

            write(fd, foto, byte);
            write(STDOUT_FILENO, "Imagen recibida\n", sizeof("Imagen recibida\n"));
            md5sum = calcularMd5sum(path);
            //if (strcmp(checksum, md5sum) == 0) {
            if (checkMD5(checksum, md5sum) == 0) {
                write(STDOUT_FILENO, "Respondiendo trama de imagen a \0",
                      sizeof("Respondiendo trama de imagen a \0"));
                write(STDOUT_FILENO, nomEstacio, strlen(nomEstacio));
                write(STDOUT_FILENO, "\n", sizeof("\n"));

                buffer2[14] = 'S';
                i = 15;
                for (int j = 0; i < TRAMA && j < (int) strlen(imagenOk); i++, j++) {
                    buffer2[i] = imagenOk[j];
                }
                for (int j = i; j < TRAMA; j++) {
                    buffer2[j] = '\0';
                }
            } else {
                write(STDOUT_FILENO, "Respondiendo trama de imagen erronea a \0",
                      sizeof("Respondiendo trama de imagen erronea a \0"));
                write(STDOUT_FILENO, nomEstacio, strlen(nomEstacio));
                write(STDOUT_FILENO, "\n", sizeof("\n"));

                buffer2[14] = 'R';
                i = 15;
                for (int j = 0; i < TRAMA && j < (int) strlen(imagenKo); i++, j++) {
                    buffer2[i] = imagenKo[j];
                }
                for (int j = i; j < TRAMA; j++) {
                    buffer2[j] = '\0';
                }
                remove(path);
            }
            write(STDOUT_FILENO, "-----------------------\n", sizeof("-----------------------\n"));

            close(fd);
            enviat = 1;
        } else {
            if (buffer[14] == 'D') {
                write(STDOUT_FILENO, "Enviando trama de datos erronea a \0",
                      sizeof("Enviando trama de datos erronea a \0"));
                write(STDOUT_FILENO, nomEstacio, strlen(nomEstacio));
                write(STDOUT_FILENO, "\n", sizeof("\n"));
                buffer2[14] = 'K';
                i = 15;
                for (int j = 0; i < TRAMA && j < (int) strlen(dadesKO); i++, j++) {
                    buffer2[i] = dadesKO[j];
                }
                for (int j = i; j < TRAMA; j++) {
                    buffer2[j] = '\0';
                }
                enviat = 1;
            }
        }

        //Responder a la trama erronea
        if (enviat == 0) {
            write(STDOUT_FILENO, "Enviando trama erronea a \0", sizeof("Enviando trama erronea a \0"));
            write(STDOUT_FILENO, nomEstacio, sizeof(strlen(nomEstacio)));
            write(STDOUT_FILENO, "\n", sizeof("\n"));
            buffer2[14] = 'Z';
            i = 15;
            for (int j = 0; i < TRAMA && j < (int) strlen(error); i++, j++) {
                buffer2[i] = error[j];
            }
            for (int j = i; j < TRAMA; j++) {
                buffer2[j] = '\0';
            }
        }
        if (enviat == 1) {
            write(*newsock, buffer2, TRAMA);
            write(STDOUT_FILENO, "Waiting...\n", sizeof("Waiting...\n"));
            read(*newsock, buffer, TRAMA);
        }
        if (enviat == 0) {
            read(*newsock, buffer, TRAMA);
        }
        //Trama de desconnexion
        if (buffer[14] == 'Q') {
            write(STDOUT_FILENO, "Recibiendo trama de desconexion de \0",
                  sizeof("Recibiendo trama de desconexion de \0"));
            write(STDOUT_FILENO, nomEstacio, sizeof(strlen(nomEstacio)));
            write(STDOUT_FILENO, "\n", sizeof("\n"));
        }
        free(md5sum);
    }
    return NULL;
}

/*
* Captura el SIGINT i ordena la senyal de tancar els threads
*/
void signalHandler() {
    write(STDOUT_FILENO, "\nDisconnecting Wendy...\n", sizeof("\nDisconnecting Wendy...\n"));
    cerrarThread = EXIT_FAILURE;
}

/*
* Configura el servidor Wendy
*/
void configurarServidor(int portJack) {
    int numClientes = 0;
    int newsock[NUM_CLIENTES];
    int sockfd = -1;
    pthread_t threadClientes[NUM_CLIENTES];
    uint16_t port;
    struct sockaddr_in s_addr;

    signal(SIGINT, signalHandler);
    port = portJack;

    sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
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

    listen(sockfd, NUM_CLIENTES);
    struct sockaddr_in c_addr;
    socklen_t c_len = sizeof(c_addr);

    write(STDOUT_FILENO, "$Wendy:\n", sizeof("$Wendy:\n"));
    write(STDOUT_FILENO, "Waiting...\n", sizeof("Waiting...\n"));

    while (cerrarThread == EXIT_SUCCESS) {

        newsock[numClientes] = accept(sockfd, (void *) &c_addr, &c_len);
        if (newsock[numClientes] > 0) {
            pthread_create(&threadClientes[numClientes], NULL, TareasServidor, (void *) &newsock[numClientes]);
            numClientes++;
            //perror("accept");
            //exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < numClientes; i++) {
        pthread_cancel(threadClientes[i]);
    }

    for (int i = 0; i < numClientes; i++) {
        pthread_join(threadClientes[i], NULL);
    }
    for (int i = 0; i < numClientes; i++) {
        close(newsock[i]);
    }
    close(sockfd);
}