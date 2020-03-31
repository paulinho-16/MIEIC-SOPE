#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "utils.h"
#include "recursive.h"

#define INF 99999


bool all=false,bytes=false,block_size=false,count_links=false,dereference=false,separate_dirs=false,max_depth=false;

int recursive_tree();

int main(int argc, char* argv[], char* envp[]) {

  if (argc < 2 || argc > 9) {
    print_usage();
    exit(1);
  }

  setenv("LOG_FILENAME","log.txt",1);

  const char *opts[]={"-a","-b","-B","-l","-L","-S","--all","--bytes","--block-size=","--count-links","--dereference","--separate-dirs","--max-depth="};
  int opts_size = sizeof(opts)/sizeof(opts[0]);
  int fd, depth=INF;
  char dirpath[100];
  
  //opens file descriptor to a log of all processes (instant - pid - action - info)
  //nothing is being written to log yet
  if(fd=open(getenv("LOG_FILENAME"),O_CREAT | O_WRONLY | O_APPEND ,0644)<0)
    perror("Error opening fd");


  //sets path and sets flags(all,bytes,block_size...) accordingly to the given command
  //the flags still aren't being used for anything
  for(int i = 1 ; i < argc ; i++) {
    if(validOption(opts,opts_size,argv[i])){
      setOption(argv[i]);      
    }
    else if(strcmp(argv[i-1],"-B") != 0){
      strcpy(dirpath,argv[i]);
    }
  }
  
  char fullpath[256];
  strcpy(fullpath, dirpath);

  if (inicial_directory(dirpath) != 0) {
    perror("inicial_directory");
    exit(2);
  }
  int result = recursive_tree(dirpath, fullpath);

  return 0;
}