#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#ifndef LOG_H
#define LOG_H

float currentTime();

int initLog();

int writeLog(char * action, char* info);

int closeLog();

#endif