#define _GNU_SOURCE

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
#include <semaphore.h>

#include "utilsQ2.h"

#define MAX_ROOM_NUMBERS 100

// Global Variables
char server_fifo[256];
int nsecs, nplaces=-1, nthreads=-1;
time_t final;
int fd_server = -1;

sem_t sem;
pthread_mutex_t mutexPlaces = PTHREAD_MUTEX_INITIALIZER;
struct room *rooms[MAX_ROOM_NUMBERS];

struct room{
    int doorNumber;
    bool occupied;
};

struct msg
{
    int i, pl;
    double dur;
    pid_t pid;
    pthread_t tid;
};

struct room * getEmptyRoom(){
    for(int i=0;i<nplaces;i++){
        if(rooms[i]->occupied==false)
            return rooms[i];
    }

    perror("reached end of getEmptyRoom\n");
    return NULL;
}

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
    struct room *bathroom;

    int fd_client;

    if(nplaces!=-1)
        rec.pl=-1;

    printf("%lu ; %d ; %d ; %lu ; %f ; %d ; RECVD\n",time(NULL),rec.i,rec.pid,rec.tid,rec.dur,rec.pl);

    if(nplaces!=-1){
        sem_wait(&sem);
        pthread_mutex_lock(&mutexPlaces);
        bathroom=getEmptyRoom();
        rec.pl=bathroom->doorNumber;
        bathroom->occupied=true;
        pthread_mutex_unlock(&mutexPlaces);
    }

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

    if(nplaces!=-1){
        sem_post(&sem);
        pthread_mutex_lock(&mutexPlaces);
        bathroom->occupied=false;
        pthread_mutex_unlock(&mutexPlaces);
    }

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
    
    if(nplaces!=-1){
        sem_init(&sem,0,nplaces);
        for(int i=1;i<=nplaces;i++){
            struct room *r = (struct room *)malloc(sizeof(struct room));
            r->doorNumber=i;
            r->occupied=false;
            rooms[i-1]=r;
        }
    }

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