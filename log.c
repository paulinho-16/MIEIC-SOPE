#include "log.h"

#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static struct timeval beginTime;
bool existsLog = true;
int myLog;

float currentTime()
{
    struct timeval thisTime;
    gettimeofday(&thisTime, NULL);
    return (thisTime.tv_usec - beginTime.tv_usec)/1000+(thisTime.tv_sec - beginTime.tv_sec)*1000;

}



int initLog(){


	char *logFileName = getenv("LOG_FILENAME");

	if(logFileName == NULL)
    {
        existsLog=false;
        return 0;
	}

	
	if((myLog = open(logFileName, O_WRONLY | O_CREAT | O_TRUNC | O_APPEND | O_SYNC, S_IRUSR | S_IWUSR)) < 0)
    {
		fprintf(stderr, "unable to open log file\n");
		return -1;
	}

    
    gettimeofday(&beginTime, NULL);	

    if((beginTimeFile = open(beginTimeFileName, O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, S_IRUSR | S_IWUSR)) < 0) //create beginTimeFileName
    {
		fprintf(stderr, "unable to open beginTime file\n");
		return -1;
	}

    if (write(beginTimeFile, &beginTime, sizeof(struct timeval))<0)
    {
        fprintf(stderr, "unable to write to beginTime file\n");
		return -1;
    }

    writeLog("CREATE", /*arguments of the call*/);


}


void writeLog(char * action,char* info)
{
    if(existsLog)
    {
        char* logLine;
        int size = sprintf(logLine, "%f - %d - %s - %s \n", currentTime(), getpid(), action, info);

        if (write(myLog, &logLine, size)<0)
        {
            fprintf(stderr, "unable to write to log file\n");
		    return -1;
        }
    }

}

int closeLog()
{
	


}