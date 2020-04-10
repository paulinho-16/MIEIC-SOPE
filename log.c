#include "log.h"

static struct timespec beginTime;
clock_t begin_time;
bool existsLog = true;
int myLog;

double begin;
//clock_t begin;

float currentTime()
{
    struct timespec thisTime;
    clock_gettime(CLOCK_MONOTONIC, &thisTime);
    /*gettimeofday(&thisTime, NULL);
    float end = thisTime.tv_usec/1000.0 + thisTime.tv_sec*1000;*/
    //clock_t end = clock();
    double end = (thisTime.tv_nsec/1000000000.0 + thisTime.tv_sec)*1000.0;
    //printf("BEGIN: %f\n", begin);
    //printf("ATUAL: %f\n", end);
    return (double) end-begin;
    //long seconds = thisTime.tv_sec - beginTime.tv_sec;
    //long micros = ((seconds*1000000) + thisTime.tv_usec) - beginTime.tv_usec;
}


int initLog(int argc, char* argv[]) {

    if (atoi(getenv("FATHER_PID")) == getpid())
    {
        char *logFileName = (char*)malloc(strlen(getenv("LOG_FILENAME"))+ 1); 
        
        logFileName = getenv("LOG_FILENAME");

        //printf("LOG_FILENAME: %s\n", logFileName);

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

        //gettimeofday(&beginTime, NULL);
        clock_gettime(CLOCK_MONOTONIC, &beginTime);
        //begin = clock();

        begin = (beginTime.tv_nsec/1000000000.0 + beginTime.tv_sec)*1000.0;
        //begin = beginTime.tv_usec/1000.0 + beginTime.tv_sec*1000;

        if (getenv("BEGIN_TIME") == NULL) {
            if(putenv("BEGIN_TIME")<0)
            {
                fprintf(stderr, "unable to create env variable\n");
                return -1;
            }
        }

        sprintf(beginString,"%f",begin);

        //printf("beginSTRING: %s\n", beginString);

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

        //printf("infoParameter: %s\n", infoParameter);

        writeLog("CREATE", infoParameter);
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
        //begin = clock();
  
        for(int i=0;i<argc-1;i++)
        {
            strcat(infoParameter,argv[i]);
            strcat(infoParameter, " ");
        }
        strcat(infoParameter,argv[argc-1]);

        writeLog("CREATE", infoParameter);

    }
}



int writeLog(char * action,char* info)
{
    if(existsLog)
    {
        //char* logLine = (char *)malloc(sizeof(float) + 3 + sizeof(int) + 3 + strlen(action) + 3 + strlen(info) + 3);
        char logLine[256];
        int size = sprintf(logLine, "%.2f - %.8d - %s - %s \n", currentTime(), getpid(), action, info);
        /*printf("Action: %s\n", action);
        printf("INFO: %s\n", info);

        printf("SIZE: %s\n", logLine);*/

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
