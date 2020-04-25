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
    int i, pid, pl;
    double dur;
    pthread_t tid;
    char fifo_client[100];
};

void *serverThread(void *arg)
{
    struct msg rec = *(struct msg *)arg;
    int fd;

    if ((fd = open(rec.fifo_client, 0666)) < 0)
        printf("Couldn't open fifo on thread\n");
    else
        printf("Processing request\n");

    sleep(rec.dur); //using bathroom
    

    rec.pid=getpid();
    rec.tid=pthread_self();

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

    if ((fd = open(server_fifo, O_RDWR)) < 0)
        printf("Couldn't open %s\n", server_fifo);
    else
        printf("%s opened in RDWR mode\n", server_fifo);

    while (1)
    {
        struct msg *request = malloc(sizeof(struct msg));

        if (read(fd, request, sizeof(struct msg)) > 0)
        {
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
}
