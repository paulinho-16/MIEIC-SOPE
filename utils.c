#include "utils.h"

extern bool all,bytes,block_size,count_links,dereference,separate_dirs,max_depth;

void print_usage() {
  printf("Usage: ./simpledu directory [options]\n\n");
  printf("Available Options:\n");
  printf(" -a / --all  -> write counts for all files, not just directories\n");
  printf(" -b / --bytes  -> presents the real number of bytes used by files or directories\n");
  printf(" -B / --block-size=[SIZE]  -> scale sizes by SIZE before printing them\n");
  printf(" -l / --count-links  -> count sizes many times if hard linked\n");
  printf(" -L / --dereference  -> dereference all symbolic links\n");
  printf(" -S / --separate-dirs  -> for directories do not include size of subdirectories\n");
  printf(" --max-depth=[N]  -> limits information displayed at N levels of directory depth\n");
}

bool validOption(const char** optionList, size_t size, char* option){
  for(int i = 0; i < size; i++) {
    if(strcmp(optionList[i], option)==0)
      return true;
  }
  return false;
}

bool setOption(char *option) {
  if(strcmp(option,"-a") == 0 || strcmp(option,"--all") == 0) {
    all=true;
    return true;
  }
  if(strcmp(option,"-b") == 0 || strcmp(option,"--bytes") == 0) {
    bytes=true;
    return true;
  }
  if(strcmp(option,"-B") == 0 || strncmp(option,"--block-size=",14) == 0) {
    block_size=true;
    return true;
  }
  if(strcmp(option,"-l") == 0 || strcmp(option,"--count-links") == 0) {
    count_links=true;
    return true;
  }
  if(strcmp(option,"-L") == 0 || strcmp(option,"--dereference") == 0) {
    dereference=true;
    return true;
  }
  if(strcmp(option,"-S") == 0 || strcmp(option,"--separate-dirs") == 0) {
    separate_dirs=true;
    return true;
  }
  if(strncmp(option,"--max-depth=",12) == 0) {
    max_depth=true;
    return true;
  }

  return false;
}

int str_length(double n)
{
  int num =1;

  n=n/10;

  while(n!=0) 
  { 
    num++;
    n=n/10;
  }
  
  return num;
}