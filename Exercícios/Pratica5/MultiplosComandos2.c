#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

#define MAX_COMMAND_NUM 10
#define MAX_COMMAND_SIZE 200
#define MAX_TOKEN_SIZE 200
#define MAX_TOKEN_NUM 200

#define READ 0
#define WRITE 1


int main(int argc, char* argv[]){
  int tokenCnt = 0;
  int cmdCnt = 0;

  // String de input original, que contém todos os comandos introduzidos pelo Utilizador
  char* commandsStr = malloc(sizeof(char) * MAX_COMMAND_SIZE);
  // Array de Strings correspondentes a cada comando a executar, resulta da decomposição de commandsStr
  char** commands = malloc(sizeof(char*) * MAX_COMMAND_NUM);
  // Array 2d que contém para cada comando (linha) os seus tokens (coluna)
  char*** tokens =  malloc(sizeof(char*) * MAX_COMMAND_NUM);
  for (int i = 0; i< MAX_COMMAND_NUM; i++) {
    tokens[i] = (char **) malloc(MAX_TOKEN_NUM*sizeof(char **));
    for (int j = 0; j < MAX_TOKEN_NUM; j++)
      tokens[i][j] = (char *)malloc(MAX_TOKEN_SIZE*sizeof(char *));
  }

  printf("Enter the command: ");
  char* validOp = fgets(commandsStr,MAX_COMMAND_SIZE*MAX_COMMAND_NUM, stdin);

  commandsStr[strlen(commandsStr)-1] = '\0';
  if(validOp == NULL)
    return 2;

  // Decompõe a String de input (commandsStr) nos vários comandos que a compõe
  commands[cmdCnt] = strtok(commandsStr, "|");
  cmdCnt++;

  while(true) {
    commands[cmdCnt] = strtok(NULL, "|");
    if(commands[cmdCnt] == NULL)
      break;
    cmdCnt++;
  }

  // Decompõe cada comando nos elementos que o compõe
  for (int i = 0; i < cmdCnt; i++) {
    commandsStr = commands[i];
    tokenCnt = 0;
    
    tokens[i][tokenCnt] = strtok(commandsStr, " ");
    tokenCnt++;
    while (true) {
      tokens[i][tokenCnt] = strtok(NULL, " ");
      if (tokens[i][tokenCnt] == NULL)
        break;
      tokenCnt++;
    }
  }

  int tempFD = dup(STDIN_FILENO);
  int* pipes = malloc(2*sizeof(int)*cmdCnt);

  for(int i = 0; i < cmdCnt;i++) {
    int* currentPipe = pipes + i * 2;
    pipe(currentPipe);
  }

  for(int i = 0; i < cmdCnt; i++) {
    int* currentPipe = pipes + i * 2;
    int* nextPipe = pipes + i * 2 + 2;

    pid_t PID = fork();
    if(PID == 0) {
      dup2(currentPipe[READ],STDIN_FILENO);
      close(currentPipe[READ]);
      if(i == 0){
        dup2(tempFD,STDIN_FILENO);
        close(tempFD);
      }
      if(i != cmdCnt -1) {
        dup2(nextPipe[WRITE],STDOUT_FILENO);
        close(nextPipe[WRITE]);
      }
      else
        close(currentPipe[WRITE]);
      execvp(tokens[i][0],tokens[i]);
    }
    close(currentPipe[READ]);
    close(currentPipe[WRITE]);
    close(nextPipe[WRITE]);
    dup2(tempFD,STDIN_FILENO);
    if (PID == 0)
      exit(0);
  }
  return 0;
}
