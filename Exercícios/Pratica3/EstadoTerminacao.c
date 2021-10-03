#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include<signal.h>

int main(int argc, char *argv[], char *envp[]) {
  pid_t pid;
  char *cmd[] = {"ls", "-laR", argv[1], NULL};
  int status;
  if (argc != 2) {
    printf("usage: %s dirname\n",argv[0]);
    exit(1);
  }
  pid=fork();
  if (pid > 0) {
    printf("My child is going to execute command \"ls -laR %s\"\n", argv[1]);
    wait(&status);
    printf("My child with pid %d terminated with exit code %d\n", pid, WEXITSTATUS(status));
    if (WIFEXITED(status))
      printf("Child terminated normally\n");
    if (WIFSIGNALED(status))
      printf("Child terminated anormally with signal %d\n", WTERMSIG(status));
  }
  else if (pid == 0) {
    // Para executar o kill, escrever "kill pid" na consola em que pid é aqui indicado.
    printf("Im Child, my pid is %d\n", getpid());
    sleep(5); // Este sleep permite dar algum tempo de copiar o pid do ecrã para comando kill
    execve("/bin/ls", cmd, envp);
    printf("Command not executed !\n");
    exit(1);
  }
  exit(0);
}