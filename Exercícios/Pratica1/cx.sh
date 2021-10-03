#!/bin/bash

# Necessário escrever chmod +x cx na consola, para conceder direitos de execução

if [ "$1" == "" ]; then
  echo "Write cx filename"
  echo "Note: the name of the file in C must not contain the .c extension"
else
  rm $1 -f
  rm *.o -f
  cc -Wall -o $1 "$1.c"
  if [ "$?" != 0 ]; then
    echo "Compilation Error"
  else
    ./$1
  fi
fi