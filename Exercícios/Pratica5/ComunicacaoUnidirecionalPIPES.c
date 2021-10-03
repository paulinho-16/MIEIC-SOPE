#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

#define READ 0
#define WRITE 1

#define MAX_BUFFER_SIZE 256   // alínea c)

struct numbers {    // alínea b)
  int n1, n2;
};

int main() {
  pid_t pid;
  int status;
  int fd[2];
  if (pipe(fd) < 0) {
    fprintf(stderr, "pipe() error\n");
    exit(1);
  }
  pid = fork();
  if (pid == -1) {
    perror("fork()");
    exit(2);
  }
  else if (pid == 0) {
    //int numbers_son[2];                          // alínea a)
    //struct numbers numeros_son;                  // alínea b)
    char s1[MAX_BUFFER_SIZE], s2[MAX_BUFFER_SIZE]; // alínea c)
    int n1, n2;                                    // alínea c)
    close(fd[WRITE]);
    //read(fd[READ], numbers_son, 2*sizeof(int));               // alínea a)
    //read(fd[READ], &numeros_son, sizeof(struct numbers));     // alínea b)
    read(fd[READ], s1, sizeof(MAX_BUFFER_SIZE));                // alínea c)
    read(fd[READ], s2, sizeof(MAX_BUFFER_SIZE));                // alínea c)
    n1 = atoi(s1);      // alínea c)
    n2 = atoi(s2);      // alínea c)
    //printf("\nx + y = %d", numbers_son[0] + numbers_son[1]);  // alínea a)
    //printf("\nx + y = %d", numeros_son.n1 + numeros_son.n2);  // alínea b)
    printf("\nx + y = %d", n1 + n2);                            // alínea c)
    //printf("\nx - y = %d", numbers_son[0] - numbers_son[1]);  // alínea a)
    //printf("\nx - y = %d", numeros_son.n1 - numeros_son.n2);  // alínea b)
    printf("\nx - y = %d", n1 - n2);                            // alínea c)   
    //printf("\nx * y = %d", numbers_son[0] * numbers_son[1]);  // alínea a)
    //printf("\nx * y = %d", numeros_son.n1 * numeros_son.n2);  // alínea b)
    printf("\nx * y = %d", n1 * n2);                            // alínea c)
    //if (numbers_son[1] == 0) {    // alínea a)
    //if (numeros_son.n2 == 0) {    // alínea b)
    if (n2 == 0) {                  // alínea c)
      printf("\nError: Division by 0\n");
      close(fd[READ]);
      exit(3);
    }
    //printf("\nx / y = %f", numbers_son[0] / (double) numbers_son[1]);   // alínea a)
    //printf("\nx / y = %f", numeros_son.n1 / (double) numeros_son.n2);   // alínea b)
    printf("\nx / y = %f", n1 / (double) n2);     // alínea c)
    close(fd[READ]);
    exit(0);
  }
  else {
    //int numbers[2];             // alínea a)
    //struct numbers numeros;      // alínea b)
    char s1[MAX_BUFFER_SIZE], s2[MAX_BUFFER_SIZE]; // alínea c)
    printf("Enter two numbers:");
    printf("\nx = ");
    //scanf("%d", &numbers[0]);     // alínea a)
    //scanf("%d", &numeros.n1);     // alínea b)
    scanf("%s", s1);                // alínea c)
    printf("y = ");
    //scanf("%d", &numbers[1]);     // alínea a)
    //scanf("%d", &numeros.n2);     // alínea b)
    scanf("%s", s2);                // alínea c)
    close(fd[READ]);
    //write(fd[WRITE], numbers, 2*sizeof(int));             // alínea a)
    //write(fd[WRITE], &numeros, sizeof(struct numbers));   // alínea b)
    write(fd[WRITE], s1, sizeof(MAX_BUFFER_SIZE));          // alínea c)
    write(fd[WRITE], s2, sizeof(MAX_BUFFER_SIZE));          // alínea c)
    close(fd[WRITE]);
    waitpid(pid, &status, 0);
    printf("\nChild with PID=%d finished with exit code %d\n", pid, WEXITSTATUS(status));
  }
  return 0;
}