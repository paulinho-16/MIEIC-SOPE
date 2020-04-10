#include "recursive.h"

extern bool all,bytes,block_size,count_links,dereference,separate_dirs,max_depth;

extern long int numero_blocos;
extern long int depth;

// Display the Information about a file
void explore_file(char* path, struct stat *stat_buf,unsigned long int *total_size) {
  long int shown_size;
  if (bytes) {
    shown_size = stat_buf->st_size;
  }
  else if (block_size) {
    shown_size = ((stat_buf->st_blocks/2) * 1024) / numero_blocos;
  }
  else {
    shown_size = stat_buf->st_blocks/2;
  }

  *total_size += shown_size;

  if (all && (!max_depth || (max_depth && depth > 0)))
    printf("%-ld\t%s\n",shown_size, path);
}

// Display the Information about a directory
void explore_directory(char* path, struct dirent *direntp, struct stat *stat_buf) {
  printf("%-ld\t%s\n",stat_buf->st_blocks/2, path);
}

int recursive_tree(char* dirpath, int dir_index, int depth_index, char** argv) {
  int status;
  unsigned long int total_size=0;
  int my_pipe[2];

  DIR *dirp;
  struct dirent *direntp;
  struct stat stat_buf;

  if((dirp=opendir(dirpath)) == NULL) {
    perror("opendir ERROR:");
    exit(2);
  }

  while((direntp=readdir(dirp)) != NULL) {
    if(strcmp(direntp->d_name,".") != 0 && strcmp(direntp->d_name,"..")) { // not hidden directories

      char* current_path = (char*)malloc(strlen(dirpath) + 1 + strlen(direntp->d_name) + 1);
      strcpy(current_path, dirpath);
      if(current_path[strlen(dirpath)-1]!='/')
        strcat(current_path, "/");
      strcat(current_path, direntp->d_name);
      current_path[strlen(current_path)] = '\0';

      if(lstat(current_path, &stat_buf) == -1) {
        perror("stat ERROR");
        exit(3);
      }

      pid_t new_pid;
      int new_status;

      if(S_ISLNK(stat_buf.st_mode)){
        if (dereference){
          if(stat(current_path, &stat_buf) == -1) {
          perror("stat ERROR");
          exit(3);
          }
        }
        else {
          explore_file(current_path, &stat_buf,&total_size);
        }
      }

      if(S_ISDIR(stat_buf.st_mode)) {

        if(pipe(my_pipe) < 0) {
          perror("pipe error");
          exit(5);
        }

        new_pid = fork();
        if (new_pid == 0) {
          gid_t gid=atoi(getenv("GROUP_ID"));

          setgid(gid); //this needs sudo permissions
          
          close(my_pipe[READ]);
          if (dup2(my_pipe[WRITE], STDIN_FILENO) == -1) {
            perror("dup2");
            exit(9);
          }
          close(my_pipe[WRITE]);
          argv[dir_index] = current_path;
          if (depth_index != -1) {
            char new_depth[256];
            char depth_string[256];
            if (depth > 0)
              depth--;
            sprintf(depth_string, "%ld", depth);
            strcpy(new_depth, "--max-depth=");
            strcat(new_depth, depth_string);
            argv[depth_index] = new_depth;
          }
          execvp(argv[0], argv);
        }
        else if (new_pid > 0) {
          char* token;
          close(my_pipe[WRITE]);
          int ret;
          char buffer[1024];

          while ((ret = read(my_pipe[READ], buffer, 1024)) > 0) {
            if (!max_depth || (max_depth && depth > 0)) {
              printf("%s", buffer);
              writeLog("ENTRY", buffer, getpid());
            }
            writeLog("SEND_PIPE", buffer, getpid());
            token = strtok(buffer, "\t");
            if(!separate_dirs)
              total_size += atol(token);
          }

          close(my_pipe[READ]);
          waitpid(-1,&status,0);
          char* exit_status = (char*)malloc(sizeof(int));
          sprintf(exit_status, "%d", WEXITSTATUS(status));
          writeLog("EXIT", exit_status, new_pid);
        }
        else {
          perror("fork ERROR");
          exit(1);
        }
      }

      if(S_ISREG(stat_buf.st_mode)) {

        explore_file(current_path, &stat_buf, &total_size);
      }
    }
  }

  //standard size of directories
  if(stat(dirpath, &stat_buf) == -1) {
    perror("stat ERROR");
    exit(3);
  }
  if (bytes) {
    total_size += (stat_buf.st_blocks/2) * 1024;
  }
  else if (block_size) {
    total_size += ((stat_buf.st_blocks/2) * 1024) / numero_blocos;
  }
  else {
    total_size += stat_buf.st_blocks/2;
  }

  char total[256];
  char* dados_diretorio = (char*)malloc(strlen(total) + 1 + strlen(dirpath) + 2);
  sprintf(total, "%ld", total_size);
  sprintf(dados_diretorio, "%s\t%s\n", total, dirpath);

  struct stat buf;

  fstat(STDIN_FILENO,&buf);
  if(S_ISFIFO(buf.st_mode)){
    write(STDIN_FILENO, dados_diretorio, strlen(dados_diretorio) + 1);
    writeLog("RECV_PIPE", dados_diretorio, getpid());
    fflush(stdin);
  }
  else{
    write(STDOUT_FILENO, dados_diretorio, strlen(dados_diretorio));
    fflush(stdout);
  }
  
  free(dados_diretorio);

  closedir(dirp);

  if (atoi(getenv("FATHER_PID")) == getpid()) {
    writeLog("EXIT", "0", getpid());
  }

  exit(0);
}