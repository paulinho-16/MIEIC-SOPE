#ifndef LOG_H
#define LOG_H

#define _POSIX_C_SOURCE 200809L

#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>

//#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

float currentTime();

int initLog();

int writeLog(char * action, char* info, pid_t pid);

int closeLog();

#endif