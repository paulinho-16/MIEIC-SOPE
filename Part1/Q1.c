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
    int fd;
    struct msg rec = *(struct msg *)arg;
    sprintf(client_fifo, "/tmp/%d.%lu", rec.pid, rec.tid);
    rec.pid = getpid();
    rec.tid = pthread_self();

    printf("%lu ; %d ; %d ; %lu ; %f ; %d ; RECVD\n",time(NULL),rec.i,rec.pid,rec.tid,rec.dur,rec.pl); 

    if ((fd = open(client_fifo, O_WRONLY)) < 0){
        printf("%lu ; %d ; %d ; %lu ; %f ; %d ; GAVUP\n",time(NULL),rec.i,getpid(),pthread_self(),rec.dur,rec.pl);
        pthread_exit(0);
    }
    else{
        printf("%lu ; %d ; %d ; %lu ; %f ; %d ; ENTER\n",time(NULL),rec.i,getpid(),pthread_self(),rec.dur,rec.pl);
        if(write(fd, &rec, sizeof(struct msg))<0)
            printf("Error writing answer to client\n");
    }

    struct timespec tim;
    tim.tv_sec = 0;
    tim.tv_nsec = rec.dur * 1000000L; // Em nanoseconds

    if (nanosleep(&tim, NULL) < 0) { // Using Bathroom
        fprintf(stderr, "Nanosleep error\n");
        exit(3);
    }

    printf("%ld ; %d ; %d ; %lu ; %f ; %d ; TIMUP\n", time(NULL), rec.i, getpid(), pthread_self(), rec.dur, rec.pl);

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

    int fd,numPlace = 1;

    if ((mkfifo(server_fifo, 0666) < 0))
    {
        if (errno == EEXIST)
            printf("%s already exists\n", server_fifo);
        else
            printf("Not able to create %s\n", server_fifo);
    }

    if ((fd = open(server_fifo, O_RDONLY)) < 0)
        printf("Couldn't open %s\n", server_fifo);

    time_t final = time(NULL) + nsecs; // tempo final

    while (time(NULL) <= final)
    {
        struct msg *request = malloc(sizeof(struct msg));

        if (read(fd, request, sizeof(struct msg)) > 0)
        {
            sprintf(client_fifo, "/tmp/%d.%lu", request->pid, request->tid);
            request->pl=numPlace;

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

    // Onde devemos meter o 2LATE?? Seria só para pedidos que fossem requisitados após encerrar o QB
    printf("%ld ; %d ; %d ; %lu ; %f ; %d ; 2LATE\n", time(NULL), numPlace - 1, getpid(), pthread_self(), (float) nsecs, numPlace - 1);

    close(fd);
    unlink(server_fifo);
    pthread_exit(0);
}
