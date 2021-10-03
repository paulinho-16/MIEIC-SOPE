#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

#define READ 0
#define WRITE 1

int main() {
  pid_t pid;
  int status;
  int parent_child[2];
  int child_parent[2];
  int INT = 1;
  int FLOAT = 2;
  int INVALID = 3;

  if (pipe(parent_child) < 0) {
    fprintf(stderr, "pipe() error\n");
    exit(1);
  }
  if (pipe(child_parent) < 0) {
    fprintf(stderr, "pipe() error\n");
    exit(1);
  }

  pid = fork();
  if (pid == -1) {
    perror("fork()");
    exit(2);
  }
  else if (pid == 0) {
    int numbers_son[2];
    int result;
    double result_division;
    close(parent_child[WRITE]);
    close(child_parent[READ]);
    read(parent_child[READ], numbers_son, 2*sizeof(int));

    result = numbers_son[0] + numbers_son[1];
    write(child_parent[WRITE], &INT, sizeof(int));
    write(child_parent[WRITE], &result, sizeof(int));

    result = numbers_son[0] - numbers_son[1];
    write(child_parent[WRITE], &INT, sizeof(int));
    write(child_parent[WRITE], &result, sizeof(int));

    result = numbers_son[0] * numbers_son[1];
    write(child_parent[WRITE], &INT, sizeof(int));
    write(child_parent[WRITE], &result, sizeof(int));

    if (numbers_son[1] == 0) {
      write(child_parent[WRITE], &INVALID, sizeof(int));
    }
    else {
      result_division = numbers_son[0] / (double) numbers_son[1];
      write(child_parent[WRITE], &FLOAT, sizeof(int));
      write(child_parent[WRITE], &result_division, sizeof(double));
    }

    close(parent_child[READ]);
    close(child_parent[WRITE]);
  }
  else {
    int numbers[2];
    int calc, stat;
    double calc_division;
    close(parent_child[READ]);
    close(child_parent[WRITE]);

    printf("Enter two numbers:");
    printf("\nx = ");
    scanf("%d", &numbers[0]);
    printf("y = ");
    scanf("%d", &numbers[1]);
    write(parent_child[WRITE], numbers, 2*sizeof(int));

    read(child_parent[READ], &stat, sizeof(int));
    read(child_parent[READ], &calc, sizeof(int));
    printf("\nx + y = %d", calc);

    read(child_parent[READ], &stat, sizeof(int));
    read(child_parent[READ], &calc, sizeof(int));
    printf("\nx - y = %d", calc);

    read(child_parent[READ], &stat, sizeof(int));
    read(child_parent[READ], &calc, sizeof(int));
    printf("\nx * y = %d", calc);

    read(child_parent[READ], &stat, sizeof(int));
    if (stat == INVALID)
      printf("\nError: Division by 0\n");
    else {
      read(child_parent[READ], &calc_division, sizeof(double));
      printf("\nx / y = %f", calc_division);
    }

    close(parent_child[WRITE]);
    close(child_parent[READ]);

    waitpid(pid, &status, 0);
    printf("\nChild with PID=%d finished with exit code %d\n", pid, WEXITSTATUS(status));
  }
  return 0;
}