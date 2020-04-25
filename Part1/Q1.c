#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/file.h>
#include <string.h>

#include "utilsQ1.h"

// Global Variables
char server_fifo[256];
int nsecs, nplaces, nthreads;

struct msg
{
    int i, pl;
    double dur;
    pid_t pid;
    pthread_t tid;
    char client_fifo[256];
};

void *serverThread(void *arg)
{
    printf("ENTROU NA THREAD DE Q1\n");

    struct msg rec = *(struct msg *)arg;
    int fd;

    printf("DEPOIS DOS DADOS DE Q1\n");

    if ((fd = open(rec.client_fifo, O_WRONLY)) < 0)
        printf("Couldn't open fifo on thread\n");
    else
        printf("Processing request\n");

    printf("ANTES DO SLEEP, dur = %f\n", rec.dur);
    sleep(rec.dur); //using bathroom
    printf("DEPOIS DO SLEEP, dur = %f\n", rec.dur);

    rec.pid = getpid();
    rec.tid = pthread_self();

    printf("EM Q1 - FIFO_RECEBIDO: %s, PID: %d, DURACAO: %f\n", rec.client_fifo, rec.pid, rec.dur);

    write(fd, &rec, sizeof(struct msg));
    free((struct msg *)arg);
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    if (argc < 4 || argc > 8) {
        print_usage();
        exit(1);
    }

    if (!get_input(argc, argv)) {
        exit(2);
    }

    int fd,numPlace=0;

    if ((mkfifo(server_fifo, 0666) < 0))
    {
        if (errno == EEXIST)
            printf("%s already exists\n", server_fifo);
        else
            printf("Not able to create %s\n", server_fifo);
    }
    else
        printf("%s created successfully\n", server_fifo);

    if ((fd = open(server_fifo, O_RDONLY)) < 0)
        printf("Couldn't open %s\n", server_fifo);
    else
        printf("%s opened in RDONLY mode\n", server_fifo);

    while (1)
    {
        struct msg *request = malloc(sizeof(struct msg));

        if (read(fd, request, sizeof(struct msg)) > 0)
        {
            request->pl=numPlace;

            printf("RECEBIDO FIFO: %s\n", request->client_fifo);

            if (request->dur == 0)
            {
                printf("closing server ...\n");
                unlink(server_fifo);
                exit(0);
            }

            pthread_t thread;
            pthread_create(&thread, NULL, serverThread, request);
            numPlace++;
        }
    }

    close(fd);
    pthread_exit(0);
}
