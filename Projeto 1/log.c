#include "log.h"

static struct timespec beginTime;
bool existsLog = true;
int myLog;

double begin;

float currentTime()
{
    struct timespec thisTime;
    clock_gettime(CLOCK_MONOTONIC, &thisTime);
    double end = (thisTime.tv_nsec/1000000000.0 + thisTime.tv_sec)*1000.0;
    return (double) end-begin;
}


int initLog(int argc, char* argv[]) {

    if (atoi(getenv("FATHER_PID")) == getpid())
    {
        char *logFileName = (char*)malloc(strlen(getenv("LOG_FILENAME"))+ 1); 
        
        logFileName = getenv("LOG_FILENAME");

        char *infoParameter = (char*)malloc(300);

        char* beginString = (char*)malloc(sizeof(float)); 

        if(logFileName == NULL)
        {
            existsLog = false;
            return 0;
        }


        if((myLog = open(logFileName, O_WRONLY | O_CREAT | O_TRUNC | O_APPEND | O_SYNC, S_IRUSR | S_IWUSR)) < 0)
        {
            fprintf(stderr, "unable to open log file\n");
            return -1;
        }

        clock_gettime(CLOCK_MONOTONIC, &beginTime);

        begin = (beginTime.tv_nsec/1000000000.0 + beginTime.tv_sec)*1000.0;

        sprintf(beginString,"%f",begin);

        if(setenv("BEGIN_TIME",beginString,1)<0)
        {
            fprintf(stderr, "unable to set env variable\n");
            return -1;
        }

        for(int i=0;i<argc-1;i++)
        {
            strcat(infoParameter,argv[i]);
            strcat(infoParameter, " ");
        }
        strcat(infoParameter,argv[argc-1]);

        writeLog("CREATE", infoParameter, getpid());
    }

    else 
    {
        char *logFileName = (char*)malloc(strlen(getenv("LOG_FILENAME"))+ 1); 
    
        char *infoParameter = (char*)malloc(300);

        logFileName = getenv("LOG_FILENAME");

        if(logFileName == NULL)
        {
            existsLog=false;
            return 0;
        }

        if((myLog = open(logFileName, O_WRONLY | O_APPEND | O_SYNC, S_IRUSR | S_IWUSR)) < 0)
        {
            fprintf(stderr, "unable to open log file\n");
            return -1;
        }


        begin = atof(getenv("BEGIN_TIME"));
  
        for(int i=0;i<argc-1;i++)
        {
            strcat(infoParameter,argv[i]);
            strcat(infoParameter, " ");
        }
        strcat(infoParameter,argv[argc-1]);

        writeLog("CREATE", infoParameter, getpid());

    }
}



int writeLog(char * action,char* info, pid_t pid)
{
    if(existsLog)
    {
        if (!strcmp(action, "ENTRY") || !strcmp(action, "RECV_PIPE")) {
            strtok(info, "\n");
            strcat(info, "\\n");
        }
        char logLine[256];
        int size = sprintf(logLine, "%.2f - %.8d - %s - %s \n", currentTime(), pid, action, info);

        if (write(myLog, logLine, strlen(logLine))<0)
        {
            fprintf(stderr, "unable to write to log file\n");
		    return -1;
        }
    }
    return 0;
}

int closeLog()
{
    if(existsLog)
    {
        if(close(myLog) != 0)
        {
            fprintf(stderr, "unable to close log\n");
            return -1;
        }
    }
    return 0;
}
