#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[], char *envp[]) {
  pid_t pid;
  int fd;
  char *cmd[] = {"ls", "-laR", argv[1], NULL};
  if (argc != 2 && argc != 3) {
    printf("Usage: %s dirname OR %s dirname file\n",argv[0], argv[0]);
    exit(1);
  }
  if (argc == 3) {
    if ((fd = open(argv[2], O_WRONLY | O_CREAT, 0644)) == -1) {
      perror(argv[2]);
      exit(2);
    }
  }
  pid=fork();
  if (pid > 0)
    printf("My child is going to execute command \"ls -laR %s\"\n", argv[1]);
  else if (pid == 0) {
    if (argc == 3)
      dup2(fd, STDOUT_FILENO);
    execve("/bin/ls", cmd, envp);
    printf("Command not executed !\n");
    exit(1);
  }
  if (argc == 3)
    close(fd);
  exit(0);
}