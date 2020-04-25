#include "utils.h"

void print_usage_U1() {
  printf("Usage: ./U1 <-t nsecs> fifoname\n\n");
  printf("   nsecs -> number of seconds that the program should work\n");
  printf("   fifoname -> name of the FIFO for communication with the server\n");
}

void print_usage_Q1() {
  printf("Usage: ./Q1 <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n\n");
  printf("   nsecs -> number of seconds that the program should work\n");
  printf("   nplaces -> bathroom stocking\n");
  printf("   nthreads -> maximum number of threads to fulfill requests\n");
  printf("   fifoname -> name of the FIFO to be created by the server to fulfill requests\n");
}