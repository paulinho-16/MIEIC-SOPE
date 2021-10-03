#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_CARACTERES 256

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usage: %s <file1> <file2>\n", argv[0]);
    return 1;
  }
  int fd1, fd2, bytes_lidos, bytes_escritos;
  char buffer[MAX_CARACTERES];
  if ((fd1 = open(argv[1],O_RDONLY)) == -1) {
    perror(argv[1]);
    exit(2);
  }
  if ((fd2 = open(argv[2],O_WRONLY | O_CREAT | O_EXCL, 0644)) == -1) {
    perror(argv[2]);
    close(fd1);
    exit(3);
  }
  while ((bytes_lidos = read(fd1, buffer, MAX_CARACTERES)) > 0) {
    if ((bytes_escritos = write(fd2, buffer, bytes_lidos)) <= 0 || bytes_lidos != bytes_escritos) {
      perror(argv[2]);
      close(fd1);
      close(fd2);
      exit(4);
    }
  }
  close(fd1);
  close(fd2);
  return 0;
}