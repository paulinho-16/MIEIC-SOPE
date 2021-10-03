#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

void read_int(int fd, int* number) {
  int n;
  do {
    n = read(fd,number,sizeof(int));
  } while (n == 0);
}

int main() {
  int INT = 1;
  int FLOAT = 2;
  int INVALID = 3;
  int fd_read, fd_write;

  if (mkfifo("/tmp/fifo_req", 0660) < 0) {
    if (errno == EEXIST)
      printf("FIFO fifo_req already exists\n");
    else
      fprintf(stderr, "mkfifo() error\n");
    exit(1);
  }

  if (mkfifo("/tmp/fifo_ans", 0660) < 0) {
    if (errno == EEXIST)
      printf("FIFO fifo_ans already exists\n");
    else
      fprintf(stderr, "mkfifo() error\n");
    exit(1);
  }

  if ((fd_read = open("/tmp/fifo_req", O_RDONLY)) < 0) {
    perror("open");
    exit(2);
  }

  if ((fd_write = open("/tmp/fifo_ans", O_WRONLY)) < 0) {
    perror("open");
    exit(2);
  }

  int numbers[2];
  while (1) {
    read_int(fd_read, &numbers[0]);
    read_int(fd_read, &numbers[1]);
    if (numbers[0] == 0 && numbers[1] == 0)
      break;
    
    int result;
    double result_division;

    result = numbers[0] + numbers[1];
    write(fd_write, &INT, sizeof(int));
    write(fd_write, &result, sizeof(int));

    result = numbers[0] - numbers[1];
    write(fd_write, &INT, sizeof(int));
    write(fd_write, &result, sizeof(int));

    result = numbers[0] * numbers[1];
    write(fd_write, &INT, sizeof(int));
    write(fd_write, &result, sizeof(int));

    if (numbers[1] == 0) {
      write(fd_write, &INVALID, sizeof(int));
    }
    else {
      result_division = numbers[0] / (double) numbers[1];
      write(fd_write, &FLOAT, sizeof(int));
      write(fd_write, &result_division, sizeof(double));
    }
  }

  close(fd_read);
  close(fd_write);
  unlink("/tmp/fifo_req");
  unlink("/tmp/fifo_ans");
  return 0;
}