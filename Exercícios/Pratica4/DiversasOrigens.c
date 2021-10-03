#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <wait.h>
#include <stdbool.h>

int v = 0;
bool segundo = true;

void sigterm_handler(int signo) {
  printf("SIGTERM signal: Reached 50 values, terminating...\n");
  exit(0);
}

void sigusr_handler(int signo) {
  if (signo == SIGUSR1)
    v++;
  else if (signo == SIGUSR2)
    v--;
  segundo = true;
}

int main() {
  srand(time(NULL));
  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(3);
  }
  else if (pid == 0) {
    struct sigaction action;
    action.sa_handler = sigusr_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGUSR1, &action, NULL) < 0) {
      printf("Error installing SIGUSR1 handler\n");
      exit(1);
    }
    if (sigaction(SIGUSR2, &action, NULL) < 0) {
      printf("Error installing SIGUSR2 handler\n");
      exit(1);
    }
    int contador = 0;
    while (1) {
      if (segundo) {
        segundo = false;
        printf("%d\n", v);
        fflush(stdout);
        sleep(1);
        contador++;
        if (contador >= 50) {
          raise(SIGTERM);
        }
      }
    }
  }
  else {
    struct sigaction action_term;
    action_term.sa_handler = sigterm_handler;
    sigemptyset(&action_term.sa_mask);
    action_term.sa_flags = 0;
    if (sigaction(SIGTERM, &action_term, NULL) < 0) {
      printf("Error installing SIGTERM handler\n");
      exit(2);
    }
    pid_t terminou;
    int status;
    while ((terminou = waitpid(pid, &status, WNOHANG)) <= 0) {
      int random = rand() % 2;
      sleep(1);
      kill(pid, random == 0 ? SIGUSR1 : SIGUSR2);
    }
  }
  return 0;
}