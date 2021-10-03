#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

void sigchld_handler(int signo) {
  printf("Entering SIGCHLD handler...\n");
  while (wait(NULL) != -1);
  printf("Exiting SIGCHLD handler...\n");
}

int main(void) {
  pid_t pid;
  int i, n;
  //signal(SIGCHLD, SIG_IGN);   // alínea a)
  signal(SIGCHLD, sigchld_handler);
  for (i=1; i<=10; i++) {
    pid=fork();
    if (pid == 0){
      printf("CHILD no. %d (PID=%d) working ... \n",i,getpid());
      sleep(15); // child working ...
      printf("CHILD no. %d (PID=%d) exiting ... \n",i,getpid());
      exit(0);
    }
  }
  for (i=1 ;i<=4; i++ ) {
    printf("PARENT: working hard (task no. %d) ...\n",i);
    n=20;
    while((n=sleep(n))!=0);
    printf("PARENT: end of task no. %d\n",i);
    printf("PARENT: waiting for child no. %d ...\n",i);
  }
  exit(0);
}