#ifndef SIGNAL_HANDLERS_H
#define SIGNAL_HANDLERS_H
//#define _XOPEN_SOURCE

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

#include "log.h"

void initSignals();
void initSignalsChildren();
void sigint_handler(int signo);
void sigcont_handler(int signo);
void sigterm_handler(int signo);
void sigcont_handler_children(int signo);
void sigterm_handler_children(int signo);

#endif
