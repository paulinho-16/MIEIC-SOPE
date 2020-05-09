#include "utilsU2.h"

extern char server_fifo[256];
extern int nsecs;

void print_usage() {
  printf("Usage: ./U2 <-t nsecs> fifoname\n\n");
  printf("   nsecs -> number of seconds that the program should work\n");
  printf("   fifoname -> name of the FIFO for communication with the server\n");
}

bool validOption(const char** optionList, size_t size, char* option) {
  for(int i = 0; i < size; i++) {
    if(strcmp(optionList[i], option)==0)
      return true;
  }
  return false;
}

bool get_input(int argc, char* argv[]) {
  const char *opts[]={"-t"};
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
    else if (server_fifo[0] == '\0' && strncmp(argv[i], "-", 1)) {
      strcpy(server_fifo, argv[i]);
    }
    else {
      printf("Invalid Option: %s\n", argv[i]);
      return false;
    }
  }

  return true;
}