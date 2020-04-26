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
#include <time.h>

#include "utilsU1.h"

// Global Variables
char server_fifo[256];
int nsecs;


struct msg
{
    int i, pl;
    double dur;
    pid_t pid;
    pthread_t tid;
};


void *userThread(void *arg)
{
    char client_fifo[256];
    int fd;
    struct msg req = *(struct msg *)arg; //request
    struct msg *rec = (struct msg *)malloc(sizeof(struct msg)); //received

    req.pid = getpid();
    req.tid = pthread_self();
    sprintf(client_fifo, "/tmp/%d.%lu", req.pid, req.tid);

    req.dur = 2; //should be randomized
    req.pl = -1;

    if ((mkfifo(client_fifo, 0666) < 0))
    {
        if (errno == EEXIST)
            printf("%s already exists\n", client_fifo);
        else
            printf("Not able to create %s", client_fifo);
    }

    if ((fd = open(server_fifo, O_WRONLY)) < 0)
        printf("Not able to open %s\n", server_fifo);

    if (write(fd, &req, sizeof(req)) > 0)
        printf("%lu ; %d ; %d ; %lu ; %f ; %d ; IWANT\n",time(NULL),req.i,req.pid,req.tid,req.dur,req.pl);
    else
        printf("Error writing message\n");
    close(fd);

    if ((fd = open(client_fifo, O_RDONLY)) < 0)
        printf("Couldn't open %s'\n", client_fifo);

    if (read(fd, rec, sizeof(struct msg)) < 0)
        printf("%lu ; %d ; %d ; %lu ; %f ; %d ; FAILD\n",time(NULL),req.i,req.pid,req.tid,req.dur,req.pl);
    else if(rec->dur == -1)
        printf("%lu ; %d ; %d ; %lu ; %f ; %d ; CLOSD\n",time(NULL),req.i,req.pid,req.tid,rec->dur,rec->pl);
    else
        printf("%lu ; %d ; %d ; %lu ; %f ; %d ; IAMIN\n",time(NULL),req.i,req.pid,req.tid,rec->dur,rec->pl);
    
    unlink(client_fifo);
    free((struct msg *)arg);
    free(rec);
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        print_usage();
        exit(1);
    }

    if (!get_input(argc, argv)) {
        exit(2);
    }

    time_t final = time(NULL) + nsecs;

    struct timespec tim;
    tim.tv_sec = 0;
    tim.tv_nsec = 500000000L; // Half a second

    int i = 1;
    while(time(NULL) < final)
    {
        if (nanosleep(&tim, NULL) < 0) {
            fprintf(stderr, "Nanosleep error\n");
            exit(3);
        }
        struct msg *request = (struct msg *)malloc(sizeof(struct msg));

        request->i = i;

        pthread_t thread;
        pthread_create(&thread, NULL, userThread, request);
        i++;
    }

    pthread_exit(0);
}
