#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "utils.h"

#define INF 99999


bool all=false,bytes=false,block_size=false,count_links=false,dereference=false,separate_dirs=false,max_depth=false;


int main(int argc,char* argv[],char* envp[]) {

  setenv("LOG_FILENAME","log.txt",1);

  const char *opts[]={"-a","-b","-B","-l","-L","-S","--all","--bytes","--block-size=","--count-links","--dereference","--separate-dirs","--max-depth="};
  int opts_size=sizeof(opts)/sizeof(opts[0]);
  int fd, status, depth=INF;
  char dirpath[100];
  pid_t pid;
  
  //opens file descriptor to a log of all processes (instant - pid - action - info)
  //nothing is being written to log yet
  if(fd=open(getenv("LOG_FILENAME"),O_CREAT | O_WRONLY | O_APPEND ,0644)<0)
    perror("Error opening fd");


  //sets path and sets flags(all,bytes,block_size...) accordingly to the given command
  //the flags still aren't being used for anything
  for(int i=1;i<argc;i++){
    if(validOption(opts,opts_size,argv[i])){
        setOption(argv[i]);      
    }
    else if(strcmp(argv[i-1],"-B")!=0){
      strcpy(dirpath,argv[i]);
    }
  }
  

  pid=fork();

  if(pid==0){
    DIR *dirp;
    struct dirent *direntp;
    struct stat stat_buf;
    
    if((dirp=opendir(dirpath))==NULL){
      perror(dirpath);
      exit(2);
    }

    chdir(dirpath);

    while((direntp=readdir(dirp))!=NULL){
      if(strncmp(direntp->d_name,".",1)!=0){ //hidden directories
        if(stat(direntp->d_name,&stat_buf)==-1){
          perror("stat ERROR");
          exit(3);
        }

        if(S_ISDIR(stat_buf.st_mode)){ //fazer ciclo recursivo que retorna a soma dos tamanhos dos ficheiros interiores ao diretorio
          printf("%-ld - %s\n",stat_buf.st_blocks,direntp->d_name);
        }

      }
    }
    closedir(dirp);
    exit(0);
  }
  else if(pid>0){
    wait(&status);
  }
  else 
    perror("fork ERROR");


  return 0;
}