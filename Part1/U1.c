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

struct msg
{
    int i, pid, pl;
    double dur;
    pthread_t tid;
    char fifo_client[100];
};

void *userThread(void *arg)
{
    int fd;
    struct msg req = *(struct msg *)arg; //request
    struct msg *rec = (struct msg *)malloc(sizeof(struct msg)); //received
    char pid_str[20],tid_str[20];
    char path[100] = "/tmp/";

    const char server[] = "/tmp/fifo_req"; //this should be written here from main

    sprintf(pid_str, "%d", getpid());
    strcat(path, pid_str);
    strcat(path, ".");
    sprintf(tid_str, "%lu", pthread_self());
    strcat(path,tid_str);

    strcpy(req.fifo_client,path);
    req.pid=atoi(pid_str);
    req.tid=atoll(tid_str);
    req.dur=0.005; //should be randomized
    req.pl=-1;

    if ((mkfifo(req.fifo_client, 0666) < 0))
    {
        if (errno == EEXIST)
            printf("%s already exists\n", path);
        else
            printf("Not able to create %s", path);
    }
    else
        printf("%s created successfully\n", path);


    if ((fd = open(server, O_RDWR)) < 0)
        printf("Not able to open %s\n", server);
    else
        printf("%s opened in RDWR mode\n", server);

    if (write(fd, &req, sizeof(req)) > 0)
        printf("Request from %s submited to server\n", path);
    else
        printf("Error writing message\n");
    close(fd);


    if ((fd = open(req.fifo_client, O_RDWR)) < 0)
        printf("Couldn't open %s'\n", req.fifo_client);
    else
        printf("%s opened in RDWR mode\n", req.fifo_client);

    if (read(fd, rec, sizeof(rec)) < 0)
        printf("Failed to read response from server\n");
    else{
        printf("Request -- i:%d pid:%d pl:%d dur:%f tid:%lu client:%s\n",req.i,req.pid,req.pl,req.dur,req.tid,req.fifo_client);
        printf("Received -- i:%d pid:%d pl:%d dur:%f tid:%lu client:%s\n",rec->i,rec->pid,rec->pl,rec->dur,rec->tid,rec->fifo_client);
    }

    unlink(req.fifo_client);
    free((struct msg *)arg);
    free(rec);
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    // should be read from arguments
    //const char server[] = "/tmp/fifo_req";
    //double nsecs = 2;
    int numRequest=0;
 

    //make 10 requests to server instead of using time
    for (int i = 0; i < 10; i++)
    {
        struct msg *request = malloc(sizeof(struct msg));

        numRequest++;
        request->i=numRequest;

        pthread_t thread;
        pthread_create(&thread, NULL, userThread, request);
    }

    
    pthread_exit(0);
}
