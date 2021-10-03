#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/mman.h>

#define MAXELEMS 10000000 // nr. max de posicoes
#define MAXPROCESSES 100    // nr. max de threads
#define min(a, b) (a)<(b)?(a):(b)
#define SEMAPHORE "/sem4"

// Não Funciona

int npos;
int buf[MAXELEMS], val=0;                // variaveis partilhadas

void fill(void *nr, int *pos, sem_t *sem) {
  while (1) {
    sem_wait(sem);
    if (pos[0] >= npos) {
      sem_post(sem);
      return;
    }
    buf[pos[0]] = pos[1];
    pos[0]++; pos[1]++;
    sem_post(sem);
    *(int *)nr += 1;
  }
}

void verify() {
  int k;
  for (k=0; k<npos; k++) {
    if (buf[k] != k)   // detecta valores errados
      printf("ERROR: buf[%d] = %d\n", k, buf[k]);
  }
}

// Testar com ./SincronizacaoSemaforos 1000000 5, por exemplo

int main(int argc, char *argv[]) {
  int k, nproc, count[MAXPROCESSES];   // array para contagens
  //pthread_t tidf[MAXPROCESSES]; // tid dos thread
  int total;
  pid_t pid;
  int *pos = (int*)malloc(sizeof(int)*MAXELEMS);
  sem_t *sem;

  if (argc != 3) {
    printf("Usage: %s <nr_pos> <nr_thrs>\n",argv[0]);
    exit(1);
  }

  int shm_fd = shm_open("/shm4", O_CREAT | O_RDWR, 0600);
  if (shm_fd < 0) {
    perror("shm_open");
    exit(2);
  }

  //int* pos = (int*)mmap(0, sizeof(int)*MAXELEMS, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  /*if (pos == MAP_FAILED) {
    perror("mmap");
    exit(3);
  }*/

  pos[0] = 0;
  pos[1] = 0;

  npos = min(atoi(argv[1]), MAXELEMS);    //no. efectivo de posicoes
  nproc = min(atoi(argv[2]), MAXPROCESSES);  //no. efectivo de processos

  sem = sem_open(SEMAPHORE, O_CREAT, 0600, 1);
  /*if (sem == SEM_FAILED) {
    perror("sem_open");
    exit(4);
  }*/

  for (k=0; k<nproc; k++) {   // criacao das threads 'fill'
    count[k] = 0;
    int status;
    pid = fork();
    if (pid < 0) {
      perror("fork");
      exit(5);
    }
    else if (pid == 0) {
      fill(&count[k], pos, sem);
      exit(0);
    }
    else {
      wait(&status);
    }
  }
  
  total=0;
  for (k=0; k<nproc; k++) {   //espera threads 'fill'
    //pthread_join(tidf[k], NULL);
    printf("count[%d] = %d\n", k, count[k]);
    total += count[k];
  }
  printf("total count = %d\n",total);   // mostra total

  pid = fork();
  if (pid < 0) {
    perror("fork");
    exit(2);
  }
  else if (pid == 0) {
    //verify();
    exit(0);
  }

  sem_close(sem);
  sem_unlink(SEMAPHORE);
  return 0;
}