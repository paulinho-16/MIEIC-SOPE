#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
  pid_t pid;
  pid = fork();
  if (pid == -1) {
    perror("fork()");
    exit(1);
  }
  else if (pid == 0) {
    write(STDOUT_FILENO, "Hello ", 6);
    return 0;
  }
  pid = fork();
  if (pid == -1) {
    perror("fork()");
    exit(2);
  }
  else if (pid == 0) {
    write(STDOUT_FILENO, "my ", 3);
    return 0;
  }
  pid = fork();
  if (pid == -1) {
    perror("fork()");
    exit(3);
  }
  else if (pid == 0) {
    write(STDOUT_FILENO, "friends!\n", 9);
    return 0;
  }
  return 0;
}