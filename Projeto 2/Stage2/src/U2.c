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
#include <time.h>
#include <signal.h>

#include "utilsU2.h"

// Global Variables
char server_fifo[256];
int nsecs;
bool closed;
bool not_available = false;

struct msg
{
    int i, pl;
    double dur;
    pid_t pid;
    pthread_t tid;
};

void sigalarm_handler(int signo) {
    fprintf(stderr, "Client Time Ended...\n");
    pthread_exit(0);
}

void *userThread(void *arg)
{
    char client_fifo[256];
    int fd_client, fd_server;
    struct msg req = *(struct msg *)arg; //request
    struct msg *rec = (struct msg *)malloc(sizeof(struct msg)); //received

    req.pid = getpid();
    req.tid = pthread_self();
    sprintf(client_fifo, "/tmp/%d.%lu", req.pid, req.tid);

    req.dur = rand() % 41 + 10; // Random between 10 and 50 ms
    req.pl = -1;

    if ((mkfifo(client_fifo, 0666) < 0))
    {
        if (errno == EEXIST)
            fprintf(stderr, "%s already exists\n", client_fifo);
        else {
            fprintf(stderr, "Not able to create %s", client_fifo);
            pthread_exit(0);
        }
    }

    if ((fd_server = open(server_fifo, O_WRONLY)) < 0) {
        if (!not_available) {
            fprintf(stderr, "Bathroom is not available (Server FIFO not found)\n");
            not_available = true;
        }
        close(fd_server);
        unlink(client_fifo);
        free((struct msg *)arg);
        free(rec);
        pthread_exit(0);
    }

    if (write(fd_server, &req, sizeof(req)) > 0)
        printf("%lu ; %d ; %d ; %lu ; %f ; %d ; IWANT\n",time(NULL),req.i,req.pid,req.tid,req.dur,req.pl);
    else {
        printf("%lu ; %d ; %d ; %lu ; %f ; %d ; FAILD\n",time(NULL),req.i,req.pid,req.tid,req.dur,req.pl);
        unlink(client_fifo);
        pthread_exit(0);
    }

    close(fd_server);

    if ((fd_client = open(client_fifo, O_RDONLY)) < 0) {
        perror("Error opening Client FIFO");
    }

    if (read(fd_client, rec, sizeof(struct msg)) < 0) {
        printf("%lu ; %d ; %d ; %lu ; %f ; %d ; FAILD\n",time(NULL),req.i,req.pid,req.tid,req.dur,req.pl);
    }
    else if(rec->dur == -1 && rec->pl == -1) {
        printf("%lu ; %d ; %d ; %lu ; %f ; %d ; CLOSD\n",time(NULL),req.i,req.pid,req.tid,rec->dur,rec->pl);
        closed = true;
    }
    else
        printf("%lu ; %d ; %d ; %lu ; %f ; %d ; IAMIN\n",time(NULL),req.i,req.pid,req.tid,rec->dur,rec->pl);
    
    close(fd_client);
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

    struct sigaction conf_sinal;
    conf_sinal.sa_handler = sigalarm_handler;
    sigemptyset(&conf_sinal.sa_mask);
    conf_sinal.sa_flags = 0;
    if (sigaction(SIGALRM, &conf_sinal, NULL) < 0) {
        fprintf(stderr,"Unable to install SIGALARM handler (sigaction)\n");
        exit(3);
    }

    alarm(nsecs);

    int i = 1;
    closed = false;
    while(!not_available)
    {
        usleep(10000);  // 10 ms entre cada pedido
        struct msg *request = (struct msg *)malloc(sizeof(struct msg));

        request->i = i;

        pthread_t thread;
        pthread_create(&thread, NULL, userThread, request);
        i++;
    }

    pthread_exit(0);
}