// PROGRAMA p04a.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

// alínea a)
// O principal inconveniente tem a ver com o tempo de execução. Como não se sabe ao certo quais 
// filhos/trabalho do pai vai demorar mais, pode levar a que se perca muito tempo a esperar por
// um filho em específico, ou um filho a esperar muito tempo para ver o seu exit code recolhido
// devido ao pai estar a demorar muito nas suas tarefas.

// alínea c)
// Com CTRL+C o pai é terminado assim como os filhos por ele criados. Ao enviar o SIGINT só ao
// processo-pai, este acaba mas os processos-filho continuam em execução.

int main(void) {
  pid_t pid;
  int i, n, status;
  for (i=1; i<=3; i++) {
    pid=fork();
    if (pid == 0){
      printf("CHILD no. %d (PID=%d) working ... \n",i,getpid());
      sleep(i*7); // child working ...
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
    //pid=wait(&status);   // alínea a)
    pid = waitpid(-1, &status, WNOHANG);
    while (pid != 0 && pid != -1) {
      if (pid != -1)
        printf("PARENT: child with PID=%d terminated with exit code %d\n",pid,WEXITSTATUS(status));
      pid = waitpid(-1, &status, WNOHANG);
    }
  }
  exit(0);
}