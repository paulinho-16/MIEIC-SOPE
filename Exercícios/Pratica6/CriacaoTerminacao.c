#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS 10

// alínea b)
// Com exit ou return não seriam garantidos todos os prints das threads criadas, como
// justificado no exercício anterior

void* HelloTid(void *threadnum) {
  printf("Hello from thread with tid = %lu!\n", pthread_self());
  sleep(1);
  pthread_exit(threadnum);
}

int main() {
  pthread_t threads[NUM_THREADS];
  int retornos[NUM_THREADS];

  for(int t = 1; t <= NUM_THREADS; t++) {
    printf("Creating thread %d\n", t);
    pthread_create(&threads[t-1], NULL, HelloTid, (void *)&t);
    retornos[t-1] = t;
  }

  // Comentar ciclo para ver efeitos da alínea b)
  for(int t = 1; t <= NUM_THREADS; t++) {
    pthread_join(threads[t-1], NULL);
    printf("Retorno da Thread: %d\n", retornos[t-1]);
  }

  pthread_exit(0);
  //return 0;     // alínea b)
}