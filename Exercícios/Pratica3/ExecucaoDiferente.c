// PROGRAMA p8.c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]) {
  pid_t pid;
  char *cmd[] = {"ls", "-laR", argv[1], NULL};
  if (argc != 2) {
    printf("usage: %s dirname\n",argv[0]);
    exit(1);
  }
  pid=fork();
  if (pid > 0)
    printf("My child is going to execute command \"ls -laR %s\"\n", argv[1]);
  else if (pid == 0) {
    //execlp("ls", "ls", "-laR", argv[1], NULL);      // alínea a)
    //execl("/bin/ls", "ls", "-laR", argv[1], NULL);  // alínea b)
    //execvp("ls", cmd);    // alínea c)
    //execv("/bin/ls", cmd);  // alínea d)
    execve("/bin/ls", cmd, envp);   // alínea e)
    printf("Command not executed !\n");
    exit(1);
  }
  exit(0);
}