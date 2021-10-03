#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

#define MAX_PASSWORD_LENGTH 20

int main() {
  char password[MAX_PASSWORD_LENGTH + 1], echo = '*', caracter;
  struct termios conf_antiga, nova_conf;
  write(STDOUT_FILENO, "\nPassword: ", 11);
  tcgetattr(STDIN_FILENO, &conf_antiga);
  nova_conf = conf_antiga;
  nova_conf.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &nova_conf);
  int i = 0;
  while (i < MAX_PASSWORD_LENGTH && read(STDIN_FILENO,&caracter, 1) && caracter != '\n') {
    password[i++] = caracter;
    write(STDOUT_FILENO, &echo, 1);
  }
  password[i] = 0;
  tcsetattr(STDIN_FILENO, TCSANOW, &conf_antiga);
  write(STDOUT_FILENO, "\n\nPassword Escrita: ", 20);
  write(STDOUT_FILENO, password, strlen(password));
  write(STDOUT_FILENO, "\n\n", 2);
  return 0;
}