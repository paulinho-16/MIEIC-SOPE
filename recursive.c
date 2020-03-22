#include "recursive.h"

extern bool all,bytes,block_size,count_links,dereference,separate_dirs,max_depth;

int recursive_tree(char* dirpath) {
  pid_t pid;
  int status;

  pid=fork();

  if(pid == 0) {
    DIR *dirp;
    struct dirent *direntp;
    struct stat stat_buf;
    
    if((dirp=opendir(dirpath)) == NULL) {
      if (dirpath[0] == '\0')
        printf("No directory input\n");
      else
        printf("Invalid Option: %s\n", dirpath);
      exit(2);
    }

    if (chdir(dirpath) != 0) {
      perror("chdir");
      exit(3);
    }

    while((direntp=readdir(dirp)) != NULL) {
      if(strncmp(direntp->d_name,".",1) != 0) { // not hidden directories
        if(stat(direntp->d_name,&stat_buf) == -1) {
          perror("stat ERROR");
          exit(3);
        }

        if(S_ISDIR(stat_buf.st_mode)) { //fazer ciclo recursivo que retorna a soma dos tamanhos dos ficheiros interiores ao diretorio
          printf("%-ld - %s\n",stat_buf.st_blocks,direntp->d_name);
        }

        if(S_ISREG(stat_buf.st_mode) && all) {
          printf("%-ld - %s\n",stat_buf.st_size,direntp->d_name);
        }
      }
    }
    closedir(dirp);
    exit(0);
  }
  else if(pid>0) {
    wait(&status);
  }
  else {
    perror("fork ERROR");
  }
}