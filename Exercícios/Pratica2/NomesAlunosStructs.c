#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_BUFFER_SIZE 512

struct DadosEstudante {
  char nome[MAX_NAME_LENGTH];
  char nota[MAX_NAME_LENGTH];
};

int main(int argc, char* argv[]) {

  // Escrita

  int fd, bytes_lidos;
  char buffer[MAX_BUFFER_SIZE];
  char sair[6] = "exit\n";
  if (argc != 2) {
    printf("Usage: %s file\n", argv[0]);
    return 1;
  }
  if ((fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, 0644)) == -1) {
    perror(argv[1]);
    return 2;
  }
  int i = 1;
  write(STDOUT_FILENO, "Nome do Aluno ('exit' para sair): ", 34);
  struct DadosEstudante dados;
  while ((bytes_lidos = read(STDIN_FILENO, buffer, MAX_BUFFER_SIZE)) > 0) {
    if (i%2 == 0) {
      strncpy(dados.nota, buffer, bytes_lidos);
      write(fd, &dados, sizeof(struct DadosEstudante));
      write(STDOUT_FILENO, "Nome do Aluno ('exit' para sair): ", 34);
    }
    else {
      if (!strcmp(buffer, sair))
        break;
      strncpy(dados.nome, buffer, bytes_lidos);
      write(STDOUT_FILENO, "Nota: ", 6);
    }
    memset(buffer, 0, sizeof(buffer));
    i++;
  }
  close(fd);

  // Leitura

  if ((fd = open(argv[1], O_RDONLY)) == -1) {
    perror(argv[1]);
    return 3;
  }
  struct DadosEstudante dados_lidos;
  write(STDOUT_FILENO, "\nDados Lidos:\n\n", 15);
  while (read(fd, &dados_lidos, sizeof(struct DadosEstudante))) {
    strtok(dados_lidos.nome, "\n");
    strtok(dados_lidos.nota, "\n");
    write(STDOUT_FILENO, dados_lidos.nome, strlen(dados_lidos.nome));
    write(STDOUT_FILENO, " - ", 3);
    write(STDOUT_FILENO, dados_lidos.nota, strlen(dados_lidos.nota));
    write(STDOUT_FILENO, "\n", 1);
  }
  close(fd);
  return 0;
}