// PROGRAMA p03.c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 10

// alínea a)
// Como o parâmetro enviado a cada thread é o endereço da variável t, quaisquer alterações a
// esta variável serão efetuadas em todas as threads. Por este motivo, naquele primeiro Hello,
// apresenta-se o número 1, uma vez que o t++ foi efetuado antes deste print, daí a variável t
// ter sido alterada. Este é o mesmo motivo daqueles Hellos repetidos.

// alínea c)
// O exit, dependendo da execução, pode não gerar os prints todos, uma vez que termina o
// programa, e consequentemente, todas as threads por ele geradas, enquanto que o pthread_exit
// termina apenas a "main thread", permitindo que as outras threads continuem em execução, daí
// serem apresentados todos os prints.

void *PrintHello(void *threadnum) {
  printf("Hello from thread no. %d!\n", *(int *) threadnum);
  pthread_exit(NULL);
}

int main() {
  pthread_t threads[NUM_THREADS];
  int t;
  for(t=0; t< NUM_THREADS; t++) {
    printf("Creating thread %d\n", t);
    pthread_create(&threads[t], NULL, PrintHello, (void *)&t);
    pthread_join(threads[t], NULL);     // alínea b)
  }
  pthread_exit(0);
  //exit(0);
}