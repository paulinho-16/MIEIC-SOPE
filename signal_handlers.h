#ifndef SIGNAL_HANDLERS_H
#define SIGNAL_HANDLERS_H
#define _XOPEN_SOURCE

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void initSignals();
void sigint_handler(int signo);
void sigcont_handler(int signo);
void sigterm_handler(int signo);


#endif
