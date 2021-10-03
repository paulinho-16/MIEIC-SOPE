#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 256
#define MAX_TOKENS 50
#define MAX_COMMANDS 50

int main() {
  char comando[MAX_LENGTH];
  printf("Enter the command: ");
  fgets(comando,MAX_LENGTH,stdin);
  char* token = strtok(comando, ";|\n");
  char* arr[MAX_COMMANDS];
  char* tokens[MAX_TOKENS*MAX_COMMANDS];
  int i = 0;
  while (token != NULL) {
    arr[i] = (char*) malloc(MAX_LENGTH*sizeof(char));
    arr[i] = token;
    i++;
    token = strtok(NULL,";|\n");
  }
  int k = 0;
  for (int j = 0 ; j < i ; j++) {
    token = strtok(arr[j], " \n");
    while (token != NULL) {
      tokens[k] = (char*) malloc(MAX_LENGTH*sizeof(char));
      tokens[k] = token;
      k++;
      token = strtok(NULL," \n");
    }
  }
  for (int m = 0 ; m < k ; m++) {
    printf("%s\n", tokens[m]);
  }
  return 0;
}