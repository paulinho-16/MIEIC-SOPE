#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_CARACTERES 512

int main(int argc, char* argv[]) {
  if ((argc != 2) && (argc != 3)) {
    printf("Usage: %s file1  OR  %s file1 file2\n", argv[0], argv[0]);
    return 1;
  }
  int fd1, fd2, bytes_lidos, bytes_escritos;
  char buffer[MAX_CARACTERES];
  if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
    perror(argv[1]);
    return 2;
  }
  if (argc == 3) {
    if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 0644)) == -1) {
      perror(argv[2]);
      close(fd1);
      return 3;
    }
    dup2(fd2, STDOUT_FILENO);
  }
  while ((bytes_lidos = read(fd1,buffer,MAX_CARACTERES)) > 0) {
    if ((bytes_escritos = write(STDOUT_FILENO, buffer, bytes_lidos)) <= 0) {
      perror(argv[2]);
      close(fd1);
      close(fd2);
      return 4;
    }
  }
  close(fd1);
  if (argc == 3)
    close(fd2);
  return 0;
}