// PROGRAMA p01.c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define STDERR 2
#define NUMITER 10000

// alínea c)
// Se for usada a mesma variável, como para as funções é enviado o endereço dessa variável, essa
// alteração seria efetuada em ambas as threads, sendo apenas apresentados 2s ou apenas 1s,
// conforme a ordem de armazenamento das variáveis (só 2 se for o último valor atríbuido)

void * thrfunc(void * arg) {
  int i;

  fprintf(stderr, "Starting thread %s\n", (char *) arg);
  for (i = 1; i <= NUMITER; i++) write(STDERR,arg,1);
  return NULL;
}

int main() {
  pthread_t ta, tb;

  //char a = '1', b = '2';    // alínea b) - char
  int a = '1', b = '2';       // alínea b) - int

  //pthread_create(&ta, NULL, thrfunc, "1");    // alínea a)
  pthread_create(&ta, NULL, thrfunc, &a);       // alínea b)
  //pthread_create(&tb, NULL, thrfunc, "2");    // alínea a)
  pthread_create(&tb, NULL, thrfunc, &b);       // alínea b)

  pthread_join(ta, NULL);
  pthread_join(tb, NULL);

  return 0;
}