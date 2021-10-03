#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
  int fd;
  char *text1="CCCCC";
  char *text2="DDDDD";
  if ((fd = open("f1.txt", O_WRONLY|O_SYNC|O_APPEND,0600)) == -1) {
    perror("Error opening file f1.txt");
    return 1;
  }
  getchar();
  if (write(fd,text1,5) == -1) {
    perror("Error writing text 1 to file f1.txt");
    return 2;
  }
  getchar();
  if (write(fd,text2,5) == -1) {
    perror("Error writing text2 to file f1.txt");
    return 2;
  }
  if (close(fd) == -1) {
    perror("Error closing file f1.txt");
    return 3;
  }
  return 0;
}