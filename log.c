#include "log.h"

static struct timeval beginTime;
bool existsLog = true;
int myLog;


float currentTime()
{
    struct timeval thisTime;
    gettimeofday(&thisTime, NULL);
    return (thisTime.tv_usec - beginTime.tv_usec)/1000+(thisTime.tv_sec - beginTime.tv_sec)*1000;

}


int initLog(int argc, char* argv[]){

    int beginTimeFile;

    if (*getenv("FATHER_PID") == getpid())
    {
        char *logFileName = getenv("LOG_FILENAME");
        char *beginTimeFileName = getenv("BEGINTIME_FILENAME");
        char *infoParameter;

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

        if((beginTimeFile = open(beginTimeFileName, O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, S_IRUSR | S_IWUSR)) < 0) 
        {
            fprintf(stderr, "unable to open beginTime file\n");
            return -1;
        }

        if (write(beginTimeFile, &beginTime, sizeof(struct timeval))<0)
        {
            fprintf(stderr, "unable to write to beginTime file\n");
            return -1;
        }

        close(beginTimeFile);


        for(int i=0;i<argc-1;i++)
        {
            strcat(infoParameter,argv[i]);
            strcat(infoParameter, " ");
        }
        strcat(infoParameter,argv[argc-1]);

        writeLog("CREATE", infoParameter);

        free(logFileName);
        free(beginTimeFileName); 

    }

    else 
    {
        char *logFileName = getenv("LOG_FILENAME");
        char *beginTimeFileName = getenv("BEGINTIME_FILENAME");
        char *infoParameter;

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

        if((beginTimeFile = open(beginTimeFileName, O_RDONLY, S_IRUSR | S_IWUSR)) < 0) 
        {
            fprintf(stderr, "unable to open beginTime file\n");
            return -1;
        }

        if (read(beginTimeFile, &beginTime, sizeof(struct timeval))<0)
        {
            fprintf(stderr, "unable to read from beginTime file\n");
            return -1;
        }

        close(beginTimeFile);

        for(int i=0;i<argc-1;i++)
        {
            strcat(infoParameter,argv[i]);
            strcat(infoParameter, " ");
        }
        strcat(infoParameter,argv[argc-1]);

        writeLog("CREATE", infoParameter);

        free(logFileName);
        free(beginTimeFileName); 
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