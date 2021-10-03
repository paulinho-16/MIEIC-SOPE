#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Com Struct
struct numbers {
  int n1;
  int n2;
};

void* Soma(void *numeros) {
  //int result = ((int *) numeros)[0] + ((int *) numeros)[1];   // Com Array
  int *result = (int *)malloc(sizeof(int));
  *result = ((struct numbers *) numeros)->n1 + ((struct numbers *) numeros)->n2;   // Com Struct
  pthread_exit((void *) result);
}

void* Subtracao(void *numeros) {
  //int result = ((int *) numeros)[0] - ((int *) numeros)[1];   // Com Array
  int *result = (int *)malloc(sizeof(int));
  *result = ((struct numbers *) numeros)->n1 - ((struct numbers *) numeros)->n2;   // Com Struct
  pthread_exit((void *) result);
}

void* Produto(void *numeros) {
  //int result = ((int *) numeros)[0] * ((int *) numeros)[1];   // Com Array
  int *result = (int *)malloc(sizeof(int));
  *result = ((struct numbers *) numeros)->n1 * ((struct numbers *) numeros)->n2;   // Com Struct
  pthread_exit((void *) result);
}

void* Quociente(void *numeros) {
  //double result = ((int *) numeros)[0] / (double) ((int *) numeros)[1];   // Com Array
  double *result = (double *)malloc(sizeof(double));
  *result = ((struct numbers *) numeros)->n1 / (double) ((struct numbers *) numeros)->n2;   // Com Struct
  pthread_exit((void *) result);
}

int main() {
  //int numbers[2];    // Com Array
  struct numbers numeros;
  write(STDOUT_FILENO, "Enter the first number: ", 24);
  //scanf("%d", &numbers[0]);   // Com Array
  scanf("%d", &numeros.n1);    // Com Struct
  write(STDOUT_FILENO, "Enter the second number: ", 25);
  //scanf("%d", &numbers[1]);   // Com  Array
  scanf("%d", &numeros.n2);    // Com Struct

  pthread_t tid_soma, tid_subtracao, tid_produto, tid_quociente;
  //pthread_create(&tid_soma, NULL, Soma, &numbers);    // Com Array
  pthread_create(&tid_soma, NULL, Soma, &numeros);   // Com Struct
  //pthread_create(&tid_subtracao, NULL, Subtracao, &numbers);  // Com Array
  pthread_create(&tid_subtracao, NULL, Subtracao, &numeros);   // Com Struct
  //pthread_create(&tid_produto, NULL, Produto, &numbers);    // Com Array
  pthread_create(&tid_produto, NULL, Produto, &numeros);     // Com Struct
  //pthread_create(&tid_quociente, NULL, Quociente, &numbers);  // Com Array
  pthread_create(&tid_quociente, NULL, Quociente, &numeros);   // Com Struct

  void *soma = malloc(sizeof(int));
  pthread_join(tid_soma, &soma);
  void *subtracao = malloc(sizeof(int));
  pthread_join(tid_subtracao, &subtracao);
  void *produto = malloc(sizeof(int));
  pthread_join(tid_produto, &produto);
  void *quociente = malloc(sizeof(double));
  pthread_join(tid_quociente, &quociente);

  printf("%d + %d = %d\n", numeros.n1, numeros.n2, *(int *) soma);
  printf("%d - %d = %d\n", numeros.n1, numeros.n2, *(int *) subtracao);
  printf("%d * %d = %d\n", numeros.n1, numeros.n2, *(int *) produto);
  printf("%d / %d = %f\n", numeros.n1, numeros.n2, *(double *) quociente);

  pthread_exit(0);
}