#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define STDERR 2
#define NUMITER 10000

int N = 50000;

pthread_mutex_t mut=PTHREAD_MUTEX_INITIALIZER;

void * thrfunc(void * arg) {
  void *retorno = malloc(sizeof(int));
  *(int *) retorno = 0;
  fprintf(stderr, "Starting thread %s\n", (char *) arg);
  while (N > 0) {
    write(STDERR,arg,1);
    pthread_mutex_lock(&mut);
    if (N <= 0) {   // É necessária esta verificação pois o valor pode ter sido já alterado pela outra thread
      pthread_mutex_unlock(&mut);
      break;
    }
    N--;
    pthread_mutex_unlock(&mut);
    (*(int *) retorno)++;
  }
  return retorno;
}

int main() {
  pthread_t ta, tb;
  pthread_create(&ta, NULL, thrfunc, "1");
  pthread_create(&tb, NULL, thrfunc, "2");

  void *retorno1, *retorno2;
  pthread_join(ta, &retorno1);
  pthread_join(tb, &retorno2);

  fprintf(stderr, "\nThread 1 wrote %d characters\n", *((int *)retorno1));
  fprintf(stderr, "Thread 2 wrote %d characters\n", *((int *)retorno2));
  fprintf(stderr, "Total written characters: %d\n", *((int *)retorno1) + *((int *) retorno2));

  free(retorno1);
  free(retorno2);
  return 0;
}