#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usage: %s dir arg\n", argv[0]);
    exit(1);
  }
  int fd1[2];
  int fd2[2];
  pid_t pid;

  if (pipe(fd1) < 0) {
    fprintf(stderr, "pipe() error\n");
    exit(2);
  }
  if (pipe(fd2) < 0) {
    fprintf(stderr, "pipe() error\n");
    exit(2);
  }

  pid = fork();
  if (pid == -1) {
    perror("fork()");
    exit(3);
  }
  else if (pid == 0) {
    close(fd1[WRITE]);
    pid = fork();
    if (pid == -1) {
      perror("fork()");
      exit(3);
    }
    else if (pid == 0) {
      close(fd1[READ]);
      close(fd2[WRITE]);
      dup2(fd2[READ], STDIN_FILENO);
      close(fd2[READ]);
      execlp("sort", "sort", NULL);
    }
    else {
      close(fd2[READ]);
      dup2(fd1[READ], STDIN_FILENO);
      dup2(fd2[WRITE], STDOUT_FILENO);
      close(fd1[READ]);
      close(fd2[WRITE]);
      execlp("grep", "grep", argv[2], NULL);
    }
  }
  else {
    close(fd1[READ]);
    close(fd2[READ]);
    close(fd2[WRITE]);
    dup2(fd1[WRITE], STDOUT_FILENO);
    close(fd1[WRITE]);
    execlp("ls", "ls", argv[1], "-laR", NULL);
  }

  return 0;
}