#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

#define MAX_NAME_LENGHT 50

#define READ 0
#define WRITE 1

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s file(.txt)\n", argv[0]);
    exit(1);
  }

  int fd[2];
  pid_t pid;
  //FILE* file;     // alínea a)
  if (pipe(fd) < 0) {
    fprintf(stderr, "pipe() error\n");
    exit(2);
  }

  pid = fork();
  if (pid == -1) {
    perror("fork()");
    exit(3);
  }
  else if (pid == 0) {
    close(fd[WRITE]);
    dup2(fd[READ], STDIN_FILENO);
    close(fd[READ]);
    printf("\nNomes Ordenados:\n");
    execlp("sort", "sort", NULL);
  }
  else {
    close(fd[READ]);
    //         alínea a)
    /*int status;
    char nome[MAX_NAME_LENGHT];
    if ((file = fopen(argv[1], "r")) == NULL) {
      perror("fopen()");
      exit(4);
    }
    while(fgets(nome, MAX_NAME_LENGHT, file) != NULL)
      write(fd[WRITE], nome, strlen(nome));*/

    //          alínea b)
    dup2(fd[WRITE], STDOUT_FILENO);
    execlp("cat", "cat", argv[1], NULL);
    close(fd[WRITE]);
  }

  //fclose(file);     // alínea a)
  return 0;
}