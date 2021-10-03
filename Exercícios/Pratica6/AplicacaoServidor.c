#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

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

void * send_calculations(void * arg) {
  int fd_write;
  struct resultados results;
  results.soma = (*(struct operacao *)arg).n1 + (*(struct operacao *)arg).n2;
  results.diferenca = (*(struct operacao *)arg).n1 - (*(struct operacao *)arg).n2;
  results.produto = (*(struct operacao *)arg).n1 * (*(struct operacao *)arg).n2;
  results.quociente = (*(struct operacao *)arg).n1 / (double) (*(struct operacao *)arg).n2;
  fd_write = open((*(struct operacao *)arg).fifo, O_WRONLY);
  write(fd_write, &results, sizeof(struct resultados));
  close(fd_write);
  return NULL;
}

int main() {
  int fd_read;
  struct operacao operation;
  pthread_t tid;
  mkfifo("/tmp/fifo_req", 0660);
  fd_read = open("/tmp/fifo_req", O_RDONLY);
  while (true) {
    read(fd_read, &operation, sizeof(struct operacao));
    printf("Operando1: %d\n", operation.n1);
    printf("Operando2: %d\n", operation.n2);
    printf("Nome do fifo_ans: %s\n", operation.fifo);
    if (!operation.n1 && !operation.n2)
      break;
    pthread_create(&tid, NULL, send_calculations, (void *) &operation);
  }
  close(fd_read);
  execlp("rm", "rm", "/tmp/fifo_req", NULL);
}