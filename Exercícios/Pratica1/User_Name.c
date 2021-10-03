#include <stdio.h>
#include <stdlib.h>

// Na Shell: export USER_NAME="Paulo Jorge Salgado Marinho Ribeiro"

int main(int argc, char *argv[], char *envp[])
{
  printf("Hello %s!\n", getenv("USER_NAME"));
  return 0;
}