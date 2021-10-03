#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void read_int(int fd, int* number) {
  int n;
  do {
    n = read(fd,number,sizeof(int));
  } while (n == 0);
}

void read_double(int fd, double* number) {
  int n;
  do {
    n = read(fd,number,sizeof(double));
  } while (n == 0);
}

int main() {
  int INVALID = 3;

  int fd_read, fd_write;
  int numbers[2];
  int calc, stat;
  double calc_division;

  do {
    fd_write = open("/tmp/fifo_req", O_WRONLY);
    if (fd_write == -1)
      sleep(1);
  } while (fd_write == -1);

  do {
    fd_read = open("/tmp/fifo_ans", O_RDONLY);
    if (fd_read == -1)
      sleep(1);
  } while (fd_read == -1);

  while (1) {
    printf("Enter two numbers:");
    printf("\nx = ");
    scanf("%d", &numbers[0]);
    printf("y = ");
    scanf("%d", &numbers[1]);
    write(fd_write, numbers, 2*sizeof(int));

    if (numbers[0] == 0 && numbers[1] == 0)
      break;

    read_int(fd_read, &stat);
    read_int(fd_read, &calc);
    printf("\nx + y = %d", calc);

    read_int(fd_read, &stat);
    read_int(fd_read, &calc);
    printf("\nx - y = %d", calc);

    read_int(fd_read, &stat);
    read_int(fd_read, &calc);
    printf("\nx * y = %d", calc);

    read_int(fd_read, &stat);
    if (stat == INVALID)
      printf("\nError: Division by 0\n");
    else {
      read_double(fd_read, &calc_division);
      printf("\nx / y = %f\n\n", calc_division);
    }
  }

  close(fd_read);
  close(fd_write);

  return 0;
}