#include "utilsQ1.h"

extern char server_fifo[256];
extern int nsecs, nplaces, nthreads;

void print_usage() {
  printf("Usage: ./Q1 <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n\n");
  printf("   nsecs -> number of seconds that the program should work\n");
  printf("   nplaces -> bathroom stocking\n");
  printf("   nthreads -> maximum number of threads to fulfill requests\n");
  printf("   fifoname -> name of the FIFO to be created by the server to fulfill requests\n");
}

bool validOption(const char** optionList, size_t size, char* option) {
  for(int i = 0; i < size; i++) {
    if(strcmp(optionList[i], option)==0)
      return true;
  }
  return false;
}

bool get_input(int argc, char* argv[]) {
  const char *opts[]={"-t", "-l", "-n"};
  int opts_size = sizeof(opts)/sizeof(opts[0]);

  for(int i = 1 ; i < argc ; i++) {
    if(validOption(opts,opts_size,argv[i])) {
      continue;
    }
    else if(strcmp(argv[i-1],"-t") == 0) {
      nsecs = atoi(argv[i]);
      if (nsecs <= 0) {
        fprintf(stderr, "Introduced invalid value after -t\n");
        return false;
      }
    }
    else if(strcmp(argv[i-1],"-l") == 0) {
      nplaces = atoi(argv[i]);
      if (nplaces <= 0) {
        fprintf(stderr, "Introduced invalid value after -l\n");
        return false;
      }
    }
    else if(strcmp(argv[i-1],"-n") == 0) {
      nthreads = atoi(argv[i]);
      if (nthreads <= 0) {
        fprintf(stderr, "Introduced invalid value after -n\n");
        return false;
      }
    }
    else if (server_fifo[0] == '\0') {
      strcpy(server_fifo, argv[i]);
    }
    else {
      printf("Invalid Option: %s\n", argv[i]);
      return false;
    }
  }

  return true;
}