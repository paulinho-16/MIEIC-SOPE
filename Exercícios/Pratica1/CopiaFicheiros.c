// PROGRAMA p6a.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUF_LENGTH 256
#define MAX 100

void usage(char executavel[]) {
  printf("usage: %s file1 file2\n", executavel);
  exit(3);
}

int main(int argc, char *argv[], char *envp[])
{
  if (argc != 3) {
    usage(argv[0]);
    exit(3);
  }

  FILE *src, *dst;
  char buf[BUF_LENGTH];
  if ((src = fopen(argv[1], "r")) == NULL)
  {
    printf("Error opening input file: %s\n", strerror(errno));
    printf("Error number: %d\n", errno);
    exit(1);
  }
  if ((dst = fopen(argv[2], "w")) == NULL)
  {
    perror("Error opening output file\n");
    printf("Error number: %d\n", errno);
    exit(2);
  }
  while((fgets(buf, MAX, src)) != NULL)
  {
    fputs(buf, dst);
  }
  fclose(src);
  fclose(dst);
  exit(0);  // zero é geralmente indicativo de "sucesso"
}