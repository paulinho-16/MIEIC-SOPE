// PROGRAMA p7.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Este programa permite compilar outro programa

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s file\n", argv[0]);
    exit(1);
  }
  char prog[20];
  sprintf(prog,"%s.c",argv[1]);
  execlp("gcc","gcc",prog,"-Wall","-o",argv[1],NULL); // Aqui o código é substítuído pelo do comando
  printf("Error executing execlp()\n"); // Este printf só é executado em caso de execlp ter retornado valor de erro (-1)
  exit(2);
}