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
#include "log.h"
#include "signal_handlers.h"

#define INF 99999
#define MAX_DIR_SIZE 256

bool all=false,bytes=false,block_size=false,count_links=false,dereference=false,separate_dirs=false,max_depth=false;

long int numero_blocos = 0;
long int depth = INF;

int main(int argc, char* argv[], char* envp[]) {

  if (argc < 2 || argc > 9) {
    print_usage();
    exit(1);
  }

  initSignals();

  if (getenv("FATHER_PID") == NULL)
  {/*
    if(putenv("FATHER_PID")<0) {
      fprintf(stderr, "Unable to create environment variable\n");
      exit(1);
    }*/

    char* fatherPid = (char *)malloc(sizeof(int));
    sprintf(fatherPid,"%d", getpid());

    if(setenv("FATHER_PID", fatherPid,1)<0)
    {
      fprintf(stderr, "unable to set father PID\n");
      exit(1);
    }
  }

  setenv("LOG_FILENAME","log.txt",1);

  initLog(argc, argv);

  const char *opts[]={"-a","-b","-B","-l","-L","-S","--all","--bytes","--block-size=","--count-links","--dereference","--separate-dirs","--max-depth="};
  int opts_size = sizeof(opts)/sizeof(opts[0]);
  int fd;
  char* dirpath = (char*)malloc(MAX_DIR_SIZE);
  
  //opens file descriptor to a log of all processes (instant - pid - action - info)
  //nothing is being written to log yet
  if(fd=open(getenv("LOG_FILENAME"),O_CREAT | O_WRONLY | O_APPEND ,0644)<0)
    perror("Error opening fd");


  //sets path and sets flags(all,bytes,block_size...) accordingly to the given command
  //the flags still aren't being used for anything

  int dir_index, depth_index = -1;

  for(int i = 1 ; i < argc ; i++) {
    if (is_directory(argv[i])) {
      strcpy(dirpath,argv[i]);
      dir_index = i;
    }
    else if(validOption(opts,opts_size,argv[i])){
      if (!setOption(argv[i])) {
        exit(2);
      }
      if (strncmp(argv[i],"--max-depth=",12) == 0) {
        depth_index = i;
      }
    }
    else if(strcmp(argv[i-1],"-B") == 0) {
      numero_blocos = atol(argv[i]);
      if (numero_blocos <= 0) {
        fprintf(stderr, "Introduced invalid value after -B\n");
        exit(3);
      }
    }
    else {
      printf("Invalid Option: %s\n", argv[i]);
      exit(4);
    }
  }

  if (dirpath[0] == '\0') {
    printf("No directory input\n");
    exit(5);
  }

  if (numero_blocos <= 0 && block_size) {
    fprintf(stderr, "No value introduced after -B\n");
    exit(6);
  }

  int result = recursive_tree(dirpath, dir_index, depth_index, argv);

  free(dirpath);

  closeLog();

  return 0;
}