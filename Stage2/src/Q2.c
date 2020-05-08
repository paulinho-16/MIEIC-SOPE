#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/file.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>

#include "utilsQ2.h"

// Global Variables
char server_fifo[256];
int nsecs, nplaces, nthreads;
time_t final;
int fd_server = -1;

struct msg
{
    int i, pl;
    double dur;
    pid_t pid;
    pthread_t tid;
};

void sigalarm_handler(int signo) {
    printf("In SIGALRM handler ...\n");
    close(fd_server);
    unlink(server_fifo);
    pthread_exit(0);
}

void *serverThread(void *arg)
{
    char client_fifo[256];
    struct msg rec = *(struct msg *)arg;
    bool late=false;
    sprintf(client_fifo, "/tmp/%d.%lu", rec.pid, rec.tid);
    rec.pid = getpid();
    rec.tid = pthread_self();

    int fd_client;

    printf("%lu ; %d ; %d ; %lu ; %f ; %d ; RECVD\n",time(NULL),rec.i,rec.pid,rec.tid,rec.dur,rec.pl);

    if ((fd_client = open(client_fifo, O_WRONLY)) < 0){
        printf("%lu ; %d ; %d ; %lu ; %f ; %d ; GAVUP\n",time(NULL),rec.i,getpid(),pthread_self(),rec.dur,rec.pl);
        printf("ERROR: %s\n", strerror(errno));
        pthread_exit(0);
    }
    else{
        printf("%lu ; %d ; %d ; %lu ; %f ; %d ; ENTER\n",time(NULL),rec.i,getpid(),pthread_self(),rec.dur,rec.pl);
        if(write(fd_client, &rec, sizeof(struct msg))<0)
            printf("Error writing answer to client\n");
    }

    close(fd_client);

    usleep(rec.dur * 1000);

    if(late)
        printf("%ld ; %d ; %d ; %lu ; %f ; %d ; 2LATE\n", time(NULL), rec.i, getpid(), pthread_self(), rec.dur, rec.pl);
    else
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

    int numPlace = 1;

    struct sigaction conf_sinal;
    conf_sinal.sa_handler = sigalarm_handler;
    sigemptyset(&conf_sinal.sa_mask);
    conf_sinal.sa_flags = 0;
    if (sigaction(SIGALRM, &conf_sinal, NULL) < 0) {
        fprintf(stderr,"Unable to install SIGALARM handler (sigaction)\n");
        exit(3);
    }

    alarm(nsecs);

    if ((mkfifo(server_fifo, 0666) < 0))
    {
        if (errno == EEXIST)
            printf("%s already exists\n", server_fifo);
        else
            printf("Not able to create %s\n", server_fifo);
    }

    if ((fd_server = open(server_fifo, O_RDONLY)) < 0)
        printf("Couldn't open %s\n", server_fifo);

    while (true)
    {
        struct msg *request = malloc(sizeof(struct msg));

        if (read(fd_server, request, sizeof(struct msg)) > 0)
        {
            sprintf(client_fifo, "/tmp/%d.%lu", request->pid, request->tid);
            request->pl=numPlace;

            pthread_t thread;
            pthread_create(&thread, NULL, serverThread, request);
            numPlace++;
        }
    }
}