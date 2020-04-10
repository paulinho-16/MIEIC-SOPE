#include "signal_handlers.h"


void initSignals(){

    struct sigaction actionInt,actionTerm,actionCont;
    actionInt.sa_handler = sigint_handler;
    actionTerm.sa_handler = sigterm_handler;
    actionCont.sa_handler = sigcont_handler;

    sigemptyset(&actionInt.sa_mask);
    sigemptyset(&actionTerm.sa_mask);
    sigemptyset(&actionCont.sa_mask);

    actionInt.sa_flags = 0;
    actionTerm.sa_flags = 0;
    actionCont.sa_flags = 0;

    if (sigaction(SIGINT,&actionInt,NULL) < 0)  {
        fprintf(stderr,"Unable to install SIGINT handler\n");
        exit(1);
    }

    if (sigaction(SIGTERM,&actionTerm,NULL) < 0)  {
        fprintf(stderr,"Unable to install SIGTERM handler\n");
        exit(2);
    }

    if (sigaction(SIGCONT,&actionCont,NULL) < 0)  {
        fprintf(stderr,"Unable to install SIGCONT handler\n");
        exit(4);
    }

}

void sigint_handler(int signo){
    kill(0,SIGSTOP);
    printf("Terminate program ?(y/n):\n");
    exit(3);
}

void sigcont_handler(int signo){
    kill(0,SIGCONT);
}

void sigterm_handler(int signo){
    kill(0,SIGTERM);
    exit(3);
}
