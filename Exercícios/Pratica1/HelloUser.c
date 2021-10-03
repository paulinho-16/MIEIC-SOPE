#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[])
{
  char user[] = "USER=";
  for (int i = 0 ; envp[i] != NULL ; i++) {
    if (strncmp(user,envp[i],5) == 0) {
      printf("Hello %s!\n", envp[i] + 5);
      break;
    }
  }
  return 0;
}