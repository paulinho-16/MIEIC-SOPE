#ifndef LOG_H
#define LOG_H

float currentTime();

int initLog();

void writeLog(char * action, char* info);

int closeLog();

#endif