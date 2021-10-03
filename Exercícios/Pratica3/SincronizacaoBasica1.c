#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
  pid_t pid, pidSon;
  int status;

  // alínea a)
  pid = fork();
  if (pid == -1) {
    perror("fork()");
    exit(1);
  }
  else if (pid == 0) {
    write(STDOUT_FILENO, "Hello ", 6);
    return 0;
  }
  else {
    if ((pidSon = wait(&status)) == -1) {
      perror("Error terminating son process");
      exit(2);
    }
    write(STDOUT_FILENO, "world! (a)\n", 11);
  }

  // alínea b)
  pid = fork();
  if (pid == -1) {
    perror("fork()");
    exit(1);
  }
  else if (pid == 0) {
    write(STDOUT_FILENO, "world! (b)\n", 11);
  }
  else {
    write(STDOUT_FILENO, "Hello ", 6);
  }
  return 0;
}