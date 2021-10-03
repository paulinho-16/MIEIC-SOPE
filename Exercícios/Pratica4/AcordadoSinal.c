// PROGRAMA p01a.c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

// alínea a)
// Para enviar sinal SIGINT premir CTRL + C - valor de retorno: 0
// Para enviar sinal SIGUSR1 abrir novo terminal, introduzir "ps -C AcordadoSinal" seguido de
// "kill -s USR1 pid", em que pid é mostrado após o comando ps - valor de retorno: 138

void sigint_handler(int signo) {
  printf("In SIGINT handler ...\n");
}

int main(void) {
  // alínea c)
  struct sigaction conf_sinal;
  conf_sinal.sa_handler = sigint_handler;
  sigemptyset(&conf_sinal.sa_mask);
  conf_sinal.sa_flags = 0;
  if (sigaction(SIGINT, &conf_sinal, NULL) < 0) {
    fprintf(stderr,"Unable to install SIGINT handler (sigaction)\n");
    exit(1);
  }

  // alínea a) e b)
  /*if (signal(SIGINT,sigint_handler) < 0) {
    fprintf(stderr,"Unable to install SIGINT handler (signal)\n");
    exit(1);
  }*/
  printf("Sleeping for 30 seconds ...\n");
  int tempo_restante = 30;
  while (tempo_restante) {
    tempo_restante = sleep(tempo_restante);
    if (tempo_restante)
      printf("Sinal ignorado: ainda faltam %ds\n", tempo_restante);
  }
  printf("Waking up ...\n");
  exit(0);
}