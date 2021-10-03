#include <stdio.h>
#include <stdlib.h>

void handler_1() {
  printf("Executing exit handler 1\n");
  //exit(1);   // Exit nos handlers sao ignorados
}

void handler_2() {
  printf("Executing exit handler 2\n");
  //exit(2);   // Exit nos handlers sao ignorados
}

int main() {
  atexit(handler_1);
  //atexit(handler_1);  // Possivel instalar um handler mais do que uma vez
  atexit(handler_2);
  //abort();    // Nada acontece
  printf("Main done!\n");
  return 0;
}