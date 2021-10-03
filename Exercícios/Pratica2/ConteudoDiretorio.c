// PROGRAMA p6a.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

// alínea d)
// Para criar um hardlink: ln temp temp1
// Para criar um symbolic link: ln -s temp temp2

// Com lstat():
// Verifica-se que temp e temp1 são do mesmo tipo (regulares) e têm o mesmo tamanho e mesmo i-node
// temp2 não tem o mesmo i-node e é do tipo other.

// Com stat():
// Tanto o hardlink como o symbolic link são do mesmo tipo (regulares) e têm o mesmo tamanho e i-node
// Isto acontece porque quando stat() recebe um pathname que é symbolic link, retorna informação sobre o ficheiro original, e não do link.
// Já lstat() retornaria informação sobre o link e não sobre o ficheiro original

// alínea e)
// Ao apagarmos primeiro o ficheiro temp, ainda conseguimos aceder ao conteúdo de temp1, pois um hardlink contem o conteúdo do ficheiro original
// No entanto, não conseguimos ver o conteúdo do ficheiro temp2 com cat, pois um symbolic link é como um apontador para o ficheiro original
// Como este deixa de existir, também a utilidade de temp2 deixa de ser relevante.

int main(int argc, char *argv[]) {
  DIR *dirp;
  struct dirent *direntp;
  struct stat stat_buf;
  char *str;

  if (argc != 2) {
    fprintf( stderr, "Usage: %s dir_name\n", argv[0]);
    exit(1); 
  }

  if ((dirp = opendir( argv[1])) == NULL) {
    perror(argv[1]);
    exit(2);
  }

  while ((direntp = readdir(dirp)) != NULL) {
    chdir(argv[1]);
    if (stat(direntp->d_name, &stat_buf) == -1) {
    //if (lstat(direntp->d_name, &stat_buf) == -1) {
      perror("lstat Error");
      exit(3);
    }

    if (S_ISREG(stat_buf.st_mode))
      str = "regular";
    else if (S_ISDIR(stat_buf.st_mode))
      str = "directory";
    else
      str = "other";
    printf("%-25s - %9s - %10d", direntp->d_name, str, (int) stat_buf.st_ino);
    if (strcmp(str, "regular") == 0) {
      printf("   %8d B", (int) stat_buf.st_size);
    }
    printf("\n");
  }
  closedir(dirp);
  exit(0);
} 