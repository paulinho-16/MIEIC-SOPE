#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s message\n", argv[0]);
    exit(1);
  }
  int tempo = 0;
  printf("%s - 0s\n", argv[1]);
  while (tempo < 30) {
    sleep(5);
    tempo += 5;
    printf("%s - %ds\n", argv[1], tempo);
  }
  return 0;
}