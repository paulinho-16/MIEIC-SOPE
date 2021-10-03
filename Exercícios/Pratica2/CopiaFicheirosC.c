#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usage: ./<exec_name> <file1.txt> <file2.txt>\n");
    return 1;
  }
  FILE *file1, *file2;
  if ((file1 = fopen(argv[1], "r")) == NULL) {
    printf("Error opening input file\n");
    exit(1);
  }
  if ((file2 = fopen(argv[2], "a")) == NULL) {
    printf("Error opening output file\n");
    exit(2);
  }
  fseek(file1,0,SEEK_END);
  long size = ftell(file1);
  char* linha = (char*) malloc(size*sizeof(char));
  fseek(file1,0,SEEK_SET);
  fread(linha,1,size,file1);
  fwrite(linha,1,size,file2);
  fclose(file1);
  fclose(file2);
  free(linha);
  return 0;
}