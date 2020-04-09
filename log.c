#include "log.h"

static struct timeval beginTime;
bool existsLog = true;
int myLog;

float begin;

float currentTime()
{
    struct timeval thisTime;
    gettimeofday(&thisTime, NULL);
    return thisTime.tv_usec/1000 + thisTime.tv_sec*1000 - begin;

}


int initLog(int argc, char* argv[]){

    if (atoi(getenv("FATHER_PID")) == getpid()) //changed
    {
        char *logFileName = (char*)malloc(strlen(getenv("LOG_FILENAME"))+ 1); 
        
        logFileName = getenv("LOG_FILENAME");
        char *infoParameter;// = (char*)malloc(300);

        char* beginString; //= (char*)malloc(300); 

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


        gettimeofday(&beginTime, NULL);

        begin=beginTime.tv_usec/1000 + beginTime.tv_sec*1000;

        if(putenv("BEGIN_TIME")<0)
        {
            fprintf(stderr, "unable to create env variable\n");
            return -1;
        }

        
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

        writeLog("CREATE", infoParameter);


    }

    else 
    {
        char *logFileName = (char*)malloc(strlen(getenv("LOG_FILENAME"))+ 1); 
    
        char *infoParameter; // = (char*)malloc(300);

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

        writeLog("CREATE", infoParameter);

    }


}



int writeLog(char * action,char* info)
{
    if(existsLog)
    {
        char* logLine = (char *)malloc(sizeof(float) + 3 + sizeof(int) + 3 + strlen(action) + 3 + strlen(info) + 1);
        int size = sprintf(logLine, "%f - %d - %s - %s \n", currentTime(), getpid(), action, info);

        if (write(myLog, &logLine, size)<0)
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
