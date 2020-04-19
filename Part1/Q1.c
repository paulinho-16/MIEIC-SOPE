#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/file.h>
#include <string.h>

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

int main()
{
    // should be read from arguments
    const char server[] = "/tmp/fifo_req";
    /*double nsecs=5;
    int nplaces,nthreads*/

    int fd,numPlace=0;

    if ((mkfifo(server, 0666) < 0))
    {
        if (errno == EEXIST)
            printf("%s already exists\n", server);
        else
            printf("Not able to create %s\n", server);
    }
    else
        printf("%s created successfully\n", server);

    if ((fd = open(server, O_RDWR)) < 0)
        printf("Couldn't open %s\n", server);
    else
        printf("%s opened in RDWR mode\n", server);

    while (1)
    {
        struct msg *request = malloc(sizeof(struct msg));

        if (read(fd, request, sizeof(struct msg)) > 0)
        {
            request->pl=numPlace;

            if (request->dur == 0)
            {
                printf("closing server ...\n");
                unlink(server);
                exit(0);
            }

            pthread_t thread;
            pthread_create(&thread, NULL, serverThread, request);
            numPlace++;
        }
    }
}
