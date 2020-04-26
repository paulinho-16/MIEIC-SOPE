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

    printf("%lu ; %d ; %d ; %lu ; %f ; %d ; %s\n",time(NULL),rec.i,rec.pid,rec.tid,rec.dur,rec.pl,"RECVD"); 


    if ((fd = open(client_fifo, O_WRONLY)) < 0){
        printf("%lu ; %d ; %d ; %lu ; %f ; %d ; %s\n",time(NULL),rec.i,getpid(),pthread_self(),rec.dur,rec.pl,"GAVUP");
        pthread_exit(0);
    }
    else{
         printf("%lu ; %d ; %d ; %lu ; %f ; %d ; %s\n",time(NULL),rec.i,getpid(),pthread_self(),rec.dur,rec.pl,"ENTER");
         if(write(fd, &rec, sizeof(struct msg))<0)
            printf("Error writing answer to client\n");
    }

    //printf("ANTES DO SLEEP, dur = %f\n", rec.dur);
    sleep(rec.dur); //using bathroom
    //printf("DEPOIS DO SLEEP, dur = %f\n", rec.dur);

    printf("%lu ; %d ; %d ; %lu ; %f ; %d ; %s\n",time(NULL),rec.i,rec.pid,rec.tid,rec.dur,rec.pl,"TIMUP");
        
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

    close(fd);
    unlink(server_fifo);
    pthread_exit(0);
}
