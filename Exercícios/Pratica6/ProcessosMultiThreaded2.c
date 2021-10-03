#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void * find_string(void * arg) {
  char** args = (char**) arg;
  char result[256] = "";
  FILE* fp;
  if ((fp = fopen(args[1], "r")) == NULL) {
    perror(args[1]);
    exit(2);
  }
  char* linha = NULL;
  size_t size = 32;
  int contador = 1, bytes_lidos;
  while ((bytes_lidos = getline(&linha, &size, fp)) > 0) {
    if (strstr(linha, args[0]) != NULL) {
      if (strcmp(result, "") == 0)
        sprintf(result, "%d", contador);
      else
        sprintf(result, "%s, %d", result, contador);
    }
    contador++;
  }
  fclose(fp);
  free(linha);
  if (strcmp(result, "") == 0)
    printf("The string \"%s\" is not in the file %s\n", args[0], args[1]);
  else
    printf("The string \"%s\" can be found in the file %s, in the lines %s\n", args[0], args[1], result);
  pthread_exit(0);
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    printf("Usage: %s \"string\" file\n", argv[0]);
    exit(1);
  }
  for (int i = 2 ; i < argc ; i++) {
    char** args = malloc(2 * sizeof(char *));
    args[0] = argv[1];
    args[1] = argv[i];
    pthread_t tid;
    pthread_create(&tid, NULL, find_string, args);
  }
  pthread_exit(0);
}