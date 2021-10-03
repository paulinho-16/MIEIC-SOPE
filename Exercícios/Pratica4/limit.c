#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

int alarmflag = 0;
int childflag = 0;

void sigalrm_handler(int signo) {
  printf("Reached time limit! Terminating...\n");
  alarmflag = 1;
}

void sigchld_handler(int signo) {
  printf("Child terminated\n");
  wait(NULL);
  childflag = 1;
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    printf("Usage: %s time ./program arg1 arg2 arg3...\n", argv[0]);
    exit(1);
  }

  struct sigaction action_alarm;
  action_alarm.sa_handler = sigalrm_handler;
  sigemptyset(&action_alarm.sa_mask);
  action_alarm.sa_flags = 0;
  if (sigaction(SIGALRM, &action_alarm, NULL) < 0) {
    fprintf(stderr,"Unable to install SIGALRM handler\n");
    exit(2);
  }
  action_alarm.sa_handler = sigchld_handler;
  if (sigaction(SIGCHLD, &action_alarm, NULL) < 0) {
    fprintf(stderr,"Unable to install SIGCHLD handler\n");
    exit(2);
  }

  char* args[argc-2];
  args[0] = argv[2];
  if (argc > 3) {
    for (unsigned int i = 0 ; i < argc-3 ; i++) {
      args[i + 1] = argv[3 + i];
    }
  }

  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(3);
  }
  else if (pid > 0) {
    alarm(atoi(argv[1]));
    while (!alarmflag) {
      if (childflag) { 
        return 0;
      }
    }
    kill(pid, SIGKILL);
    raise(SIGTERM);
  }
  else {
    if (argc > 3)
      execvp(argv[2], args);
    else
      execlp(argv[2], argv[2], NULL);
    perror("execvp");
    exit(4);
  }
  return 0;
}
