#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 512
#define MAX_CMD 20

int main(int argc, char* argv[]) {
  pid_t pid;
  int status, contador = 0;
  char buffer[BUFFER_SIZE];
  char * cmd[MAX_CMD];
  write(STDOUT_FILENO, "minish >", 8);
  read(STDIN_FILENO, buffer, BUFFER_SIZE);
  buffer[strcspn(buffer,"\n")] = '\0';
  if (strchr(buffer, ' ') == NULL) {
    cmd[0] = buffer;
    cmd[1] = NULL;
  }
  else {
    char* extensao;
    extensao = strtok(buffer, " ");
    while (extensao != NULL) {
      cmd[contador++] = extensao;
      extensao = strtok(NULL, " ");
    }
    cmd[contador] = NULL;
  }
  while (strcmp(buffer, "quit") != 0) {
    pid = fork();
    if (pid > 0) {
      wait(&status);
      printf("My Child with pid %d terminated with exit code %d\n", pid, WEXITSTATUS(status));
    }
    else if (pid == 0) {
      if (contador > 2 && strcmp(cmd[contador-2], "-o") == 0) {
        int fd;
        if ((fd = open(cmd[contador-1], O_WRONLY | O_CREAT, 0644)) == -1) {
          perror(argv[2]);
          exit(2);
        }
        dup2(fd, STDOUT_FILENO);
        cmd[contador-2] = NULL;
      }
      execvp(cmd[0], cmd);
      printf("Command not found!\n");
      exit(1);
    }
    else {
      perror("fork");
      exit(2);
    }
    write(STDOUT_FILENO, "minish >", 8);
    memset(buffer, 0, BUFFER_SIZE);
    read(STDIN_FILENO, buffer, BUFFER_SIZE);
    buffer[strcspn(buffer,"\n")] = '\0';
    if (strchr(buffer, ' ') == NULL) {
      cmd[0] = buffer;
      cmd[1] = NULL;
    }
    else {
      char* extensao;
      contador = 0;
      extensao = strtok(buffer, " ");
      while (extensao != NULL) {
        cmd[contador++] = extensao;
        extensao = strtok(NULL, " ");
      }
      cmd[contador] = NULL;
    }
  }
  return 0;
}