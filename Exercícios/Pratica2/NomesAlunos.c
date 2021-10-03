#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h> 

#define BUFFER_SIZE 512

int main(int argc, char* argv[]) {
  int fd, bytes_lidos, bytes_escritos;
  char buffer[BUFFER_SIZE];
  if (argc != 2) {
    printf("Usage: %s file\n", argv[0]);
    return 1;
  }
  if ((fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, 0644)) == -1) {
    perror(argv[1]);
    return 2;
  }
  int i = 1;
  char sair[6] = "exit\n";
  write(STDOUT_FILENO, "Nome do Aluno ('exit' para sair): ", 34);
  while ((bytes_lidos = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0) {
    if (i%2 == 0) {
      write(STDOUT_FILENO, "Nome do Aluno ('exit' para sair): ", 34);
    }
    else {
      if (!strcmp(buffer, sair))
        break;
      write(STDOUT_FILENO, "Nota: ", 6);
    }
    if ((bytes_escritos = write(fd, buffer, bytes_lidos)) <= 0 || bytes_escritos != bytes_lidos) {
      perror(argv[1]);
      close(fd);
      return 3;
    }
    memset(buffer,0,sizeof(buffer));
    i++;
  }
  close(fd);
  return 0; 
}