#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>

struct comando {
  char path_ficheiro_original[256];
  char diretorio_copia[256];
};

void * copy_file(void * arg) {
  pid_t pid = fork();
  if (pid == 0) {
    execlp("cp", "cp", (*((struct comando *) arg)).path_ficheiro_original, (*((struct comando *) arg)).diretorio_copia, NULL);
  }
  else if (pid < 0) {
    perror("fork");
    exit(4);
  }
  pthread_exit(0);
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usage: %s dir1 dir2\n", argv[0]);
    exit(1);
  }
  DIR *dirp;
  struct dirent *direntp;
  struct stat stat_buf;
  char diretorio_original[256];
  char diretorio_copia[256];
  if (strncmp(argv[1], "./", 2) != 0)
    sprintf(diretorio_original, "./%s", argv[1]);
  else
    strcpy(diretorio_original, argv[1]);
  if (strncmp(argv[2], "./", 2) != 0)
    sprintf(diretorio_copia, "./%s", argv[2]);
  else
    strcpy(diretorio_copia, argv[2]);
  if ((dirp = opendir(argv[1])) == NULL) {
    perror(argv[1]);
    exit(2);
  }
  while ((direntp = readdir(dirp)) != NULL) {
    char path_ficheiro[500];
    sprintf(path_ficheiro, "%s/%s", diretorio_original, direntp->d_name);
    if (stat(path_ficheiro, &stat_buf) != 0) {
      perror("stat");
      exit(3);
    }
    if (S_ISREG(stat_buf.st_mode)) {
      pthread_t tid;
      struct comando command;
      sprintf(command.path_ficheiro_original, "%s/%s", diretorio_original, direntp->d_name);
      strcpy(command.diretorio_copia, diretorio_copia);
      pthread_create(&tid, NULL, copy_file, (void *) &command);
      pthread_join(tid, NULL);
    }
  }
  closedir(dirp);
  pthread_exit(0);
}