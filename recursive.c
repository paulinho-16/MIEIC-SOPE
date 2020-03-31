#include "recursive.h"

extern bool all,bytes,block_size,count_links,dereference,separate_dirs,max_depth;

// Display the Information about a file
void explore_file(char* path, struct dirent *direntp, struct stat *stat_buf) {
  //printf("%-ld\t%s\n",stat_buf->st_size/1024, path);
  printf("%-ld\t%s\n",stat_buf->st_blocks/2, path);
}

// Display the Information about a directory
void explore_directory(char* path, struct dirent *direntp, struct stat *stat_buf) {
  printf("%-ld\t%s\n",stat_buf->st_blocks/2, path);
}

int inicial_directory(char* dirpath) {
  DIR *dirp;
  struct dirent *direntp;
  struct stat stat_buf;
  char* copy = (char *) malloc (strlen(dirpath) - 1);

  strcpy(copy, dirpath);
  if (strncmp(copy, "./", 2) == 0) {
    copy = strtok(copy, "./");
  }

  if((dirp=opendir(".")) == NULL) {
    if (dirpath[0] == '\0')
      printf("No directory input\n");
    else
      printf("Invalid Option: %s\n", dirpath);
    exit(1);
  }

  while((direntp=readdir(dirp)) != NULL) {
    if(strcmp(direntp->d_name,copy) == 0) {
      if(stat(direntp->d_name,&stat_buf) == -1) {
          perror("stat ERROR");
          exit(3);
      }
      explore_directory(dirpath, direntp, &stat_buf);
      closedir(dirp);
      return 0;
    }
  }
  closedir(dirp);
  printf("Error: Directory not found\n");
  exit(4);
}



//NEW

//Executes simpledu on the inner directory

void exec_dir(char* current_path)
{

  char *arg[10];
  int n =2;

  arg[0]= "simpledu";
  arg[1]= current_path;


  if (all)
  {
    arg[n] = "-a";
    n++;
  }

  // if(apparent_size) not yet implemented

  if (block_size /*!= default block size ?*/ )
  {
    arg[n]="-B";
    n++;
    int size = str_length(block_size);
    char block_size_string[size + 1];
    sprintf(block_size_string, "%d", block_size);
    arg[n] = block_size_string;
    n++;
  }

  if(dereference)
  {
    arg[n] = "--L";
    n++;
  }

  // if(max_depth)
  //depth--;
  //acrescentar isso à string

  if(separate_dirs)
  {
    arg[n] = "--S";
    n++;
  }

  arg[n] = NULL;


  execv(arg[0],arg); 
  
}


int recursive_tree(char* dirpath, char* fullpath) {
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

        if(S_ISDIR(stat_buf.st_mode)) { //fazer ciclo recursivo que retorna a soma dos tamanhos dos ficheiros interiores ao diretorio
          char* new_dirpath = (char*)malloc(2 + strlen(direntp->d_name) + 1);
          strcat(new_dirpath, "./");
          strcat(new_dirpath, direntp->d_name);
          new_dirpath[strlen(new_dirpath)] = '\0';

          new_pid = fork();
          if (new_pid == 0) {
            // Constroi o fullpath, a apresentar no ecrã
            strcat(fullpath, "/");
            strcat(fullpath, direntp->d_name);
            fullpath[strlen(fullpath)] = '\0';

            recursive_tree(new_dirpath, fullpath);
            exit(0);
          }
          else if (new_pid > 0) {
            char* current_path = (char*)malloc(strlen(fullpath) + 1 + strlen(direntp->d_name));
            strcpy(current_path, fullpath);
            strcat(current_path, "/");
            strcat(current_path, direntp->d_name);
            current_path[strlen(current_path)] = '\0';

            

            // CHANGE
            //explore_directory(current_path, direntp, &stat_buf);
            //while (waitpid(new_pid, &status, WNOHANG) <= 0);
          
            //printf("Child with PID=%d finished with exit code --- %d\n", new_pid, WEXITSTATUS(status));
            exec_dir(current_path);

          }
          else {
            perror("fork ERROR");
            exit(1);
          }
        }

        if(S_ISREG(stat_buf.st_mode) && all) {
          char* current_path = (char*)malloc(strlen(fullpath) + 1 + strlen(direntp->d_name));
          strcpy(current_path, fullpath);
          strcat(current_path, "/");
          strcat(current_path, direntp->d_name);
          current_path[strlen(current_path)] = '\0';

          explore_file(current_path, direntp, &stat_buf);
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