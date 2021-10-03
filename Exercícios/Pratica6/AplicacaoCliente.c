#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>

struct operacao {
  int n1;
  int n2;
  char fifo[256];
};

struct resultados {
  int soma;
  int diferenca;
  int produto;
  double quociente;
};

int main() {
  int fd_write, fd_read;
  struct operacao operation;
  sprintf(operation.fifo, "/tmp/fifo_ans_%u", getpid());
  mkfifo(operation.fifo, 0660);
  write(STDOUT_FILENO, "Starting Calculator, enter 0 twice to end\n\n", 43);
  while (true) {
    write(STDOUT_FILENO, "Enter the first number: ", 24);
    scanf("%d", &operation.n1);
    write(STDOUT_FILENO, "Enter the second number: ", 25);
    scanf("%d", &operation.n2);
    fd_write = open("/tmp/fifo_req", O_WRONLY);
    write(fd_write, &operation, sizeof(struct operacao));
    if (!operation.n1 && !operation.n2)
      break;
    fd_read = open(operation.fifo, O_RDONLY);
    struct resultados results;
    read(fd_read, &results, sizeof(struct resultados));
    printf("%d + %d = %d\n", operation.n1, operation.n2, results.soma);
    printf("%d - %d = %d\n", operation.n1, operation.n2, results.diferenca);
    printf("%d * %d = %d\n", operation.n1, operation.n2, results.produto);
    printf("%d / %d = %f\n\n", operation.n1, operation.n2, results.quociente);
  }
  close(fd_read);
  close(fd_write);
  execlp("rm", "rm", operation.fifo, NULL);
}