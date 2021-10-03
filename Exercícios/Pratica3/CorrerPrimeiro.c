// PROGRAMA p2.c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/* alínea d)
    Uma vez que consolas como o Terminal de Linux são line-buffered, enquanto não for detetado
  um caracter newline no ecrã ('\n'), o conteúdo do printf fica no buffer. Assim, o printf("1")
  apenas armazena o 1 no buffer. Logo após, é realizado o fork(), e como o processo-filho é uma
  cópia do conteúdo do processo-pai, então o buffer do processo-filho também conterá o "1", daí 
  ser apresentado no ecrã 2 vezes.
    Ao colocarmos o "\n" no printf, obrigamos a que a informação seja imediatamente imprimida no
  ecrã e a que o buffer do processo-pai seja assim esvaziado. Desta forma, "1" já não estará no 
  buffer do processo-filho, e só será apresentado no ecrã 1 vez.
*/

int main(void) {
  //write(STDOUT_FILENO,"1",1);
  printf("1\n");     
  if(fork() > 0) {
    //write(STDOUT_FILENO,"2",1);
    printf("2");
    //write(STDOUT_FILENO,"3",1);
    printf("3");
  }
  else {
    //write(STDOUT_FILENO,"4",1);
    printf("4");
    //write(STDOUT_FILENO,"5",1);
    printf("5");
  }
  //write(STDOUT_FILENO,"\n",1);
  printf("\n");
  return 0;
}