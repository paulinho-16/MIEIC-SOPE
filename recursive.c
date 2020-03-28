#include "recursive.h"

extern bool all,bytes,block_size,count_links,dereference,separate_dirs,max_depth;

// Display the Information about a file
void explore_file(char* path, struct dirent *direntp, struct stat *stat_buf) {
  printf("%-ld\t%s\n",stat_buf->st_size, path);
}

// Display the Information about a directory
void explore_directory(char* path, struct dirent *direntp, struct stat *stat_buf) {
  printf("%-ld\t%s\n",stat_buf->st_blocks, path);
}

int recursive_tree(char* dirpath) {
  pid_t main_pid;
  int status;

  main_pid=fork();

  if(main_pid == 0) {
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

        pid_t new_pid;
        int new_status;
        char* new_dirpath = (char*)malloc(2 + strlen(direntp->d_name) + 1);
        strcat(new_dirpath, "./");
        strcat(new_dirpath, direntp->d_name);
        new_dirpath[strlen(new_dirpath)] = '\0';

        if(S_ISDIR(stat_buf.st_mode)) { //fazer ciclo recursivo que retorna a soma dos tamanhos dos ficheiros interiores ao diretorio
          new_pid = fork();
          if (new_pid == 0) {
            recursive_tree(new_dirpath);
            exit(0);
          }
          else if (new_pid > 0) {
            explore_directory(new_dirpath, direntp, &stat_buf);
            wait(&new_status);
            //printf("Child with PID=%d finished with exit code %d\n", new_pid, WEXITSTATUS(status));
          }
          else {
            perror("fork ERROR");
            exit(1);
          }
        }

        if(S_ISREG(stat_buf.st_mode) && all) {
          explore_file(new_dirpath, direntp, &stat_buf);
        }
      }
    }
    closedir(dirp);
    exit(0);
  }
  else if(main_pid>0) {
    wait(&status);
    //printf("Child with PID=%d finished with exit code %d\n", main_pid, WEXITSTATUS(status));
  }
  else {
    perror("fork ERROR");
    exit(1);
  }
}