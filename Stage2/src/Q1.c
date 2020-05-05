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

#include "utilsQ1.h"

// Global Variables
char server_fifo[256];
int nsecs, nplaces, nthreads;
time_t final;

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
    bool late=false;
    sprintf(client_fifo, "/tmp/%d.%lu", rec.pid, rec.tid);
    rec.pid = getpid();
    rec.tid = pthread_self();

    struct timespec tim;
    tim.tv_sec = 0;
    tim.tv_nsec = rec.dur * 1000000L; // Em nanoseconds

    if(time(NULL)+rec.dur/1000>(double)final){
        late=true;
        rec.dur=-1;
    }

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

    if (nanosleep(&tim, NULL) < 0) { // Using Bathroom
        fprintf(stderr, "Nanosleep error\n");
        exit(3);
    }

    if(late)
        printf("%ld ; %d ; %d ; %lu ; %f ; %d ; 2LATE\n", time(NULL), rec.i, getpid(), pthread_self(), rec.dur, rec.pl);
    else
        printf("%ld ; %d ; %d ; %lu ; %f ; %d ; TIMUP\n", time(NULL), rec.i, getpid(), pthread_self(), rec.dur, rec.pl);

    free((struct msg *)arg);
    pthread_exit(0);
}

// Thread que garante que o Bathroom encerra mesmo sem ter recebido qualquer pedido
void *firstClient(void *arg){
    int *fd =  (int *)arg;

    if ((*fd = open(server_fifo, O_RDONLY)) < 0)
        printf("Couldn't open %s\n", server_fifo);

    pthread_exit(0);
}

int main(int argc, char *argv[])
{

    char client_fifo[256];
    pthread_t readThread;

    if (argc < 4 || argc > 8) {
        print_usage();
        exit(1);
    }

    if (!get_input(argc, argv)) {
        exit(2);
    }

    final = time(NULL) + nsecs; // tempo final
    int fd=-1,numPlace = 1;

    if ((mkfifo(server_fifo, 0666) < 0))
    {
        if (errno == EEXIST)
            printf("%s already exists\n", server_fifo);
        else
            printf("Not able to create %s\n", server_fifo);
    }

    pthread_create(&readThread,NULL,firstClient,&fd);

    while(fd==-1){
        if(time(NULL) > final){
            printf("Closing Bathroom ...\n");
            unlink(server_fifo);
            exit(0);
        }
    }

    while (time(NULL) <= final)
    {
        struct msg *request = malloc(sizeof(struct msg));

        if (read(fd, request, sizeof(struct msg)) > 0)
        {
            sprintf(client_fifo, "/tmp/%d.%lu", request->pid, request->tid);
            request->pl=numPlace;

            pthread_t thread;
            pthread_create(&thread, NULL, serverThread, request);
            numPlace++;
        }
    }

    close(fd);
    unlink(server_fifo);
    pthread_exit(0);
}
