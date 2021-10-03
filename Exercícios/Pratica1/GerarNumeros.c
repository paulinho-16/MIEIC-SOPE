#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Invalid number of arguments\n");
    exit(1);
  }
  int n1 = atoi(argv[1]);
  int n2 = atoi(argv[2]);
  int number, iteracao = 0;
  srand(time(NULL));
  clock_t inicio = clock();
  do {
    number = rand() % n1;
    iteracao++;
    printf("Iteration %d:\t%d\n", iteracao, number);
  } while (number != n2);
  clock_t fim = clock();
  double tempo = (double) (fim-inicio)/CLOCKS_PER_SEC;
  printf("\nTotal time: %f\n", tempo);
  // Correr na Consola: time ./Executavel n1 n2 para obter real, user e sys time.
  return 0;
}