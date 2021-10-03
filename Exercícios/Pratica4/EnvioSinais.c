#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Usage: %s exe1 exe2 exe3 ...\n", argv[0]);
    exit(1);
  }
  pid_t pid;
  int status;
  for (int i = 1 ; i < argc ; i++) {
    pid = fork();
    if (pid == -1) {
      perror("fork");
      exit(2);
    }
    else if (pid == 0) {
      execl(argv[i], argv[i], NULL);
      fprintf(stderr, "Error with execl\n");
      exit(1);
    }
  }
  while ((pid = wait(&status)) != -1) {
    printf("Child with PID=%d finished with exit code %d\n", pid, WEXITSTATUS(status));
    if (WEXITSTATUS(status) != 0)
      kill(0, SIGKILL);
  }
  return 0;
}