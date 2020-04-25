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
};

void *serverThread(void *arg)
{
    char client_fifo[256];

    printf("ENTROU NA THREAD DE Q1\n");

    struct msg rec = *(struct msg *)arg;
    int fd;

    sprintf(client_fifo, "/tmp/%d.%lu", rec.pid, rec.tid);

    printf("DEPOIS DOS DADOS DE Q1\n");

    if ((fd = open(client_fifo, O_WRONLY)) < 0)
        printf("Couldn't open fifo on thread\n");
    else
        printf("Processing request\n");

    printf("ANTES DO SLEEP, dur = %f\n", rec.dur);
    sleep(rec.dur); //using bathroom
    printf("DEPOIS DO SLEEP, dur = %f\n", rec.dur);

    rec.pid = getpid();
    rec.tid = pthread_self();

    printf("EM Q1 - FIFO_RECEBIDO: %s, PID: %d, DURACAO: %f\n", client_fifo, rec.pid, rec.dur);

    write(fd, &rec, sizeof(struct msg));
    free((struct msg *)arg);
    pthread_exit(0);
}

int main(int argc, char *argv[])
{

    char client_fifo[256];

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

    time_t final = time(NULL) + nsecs; // tempo final

    while (time(NULL) < final)
    {
        struct msg *request = malloc(sizeof(struct msg));

        if (read(fd, request, sizeof(struct msg)) > 0)
        {
            sprintf(client_fifo, "/tmp/%d.%lu", request->pid, request->tid);
            request->pl=numPlace;

            printf("RECEBIDO FIFO: %s\n", client_fifo);

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
    unlink(server_fifo);
    pthread_exit(0);
}
