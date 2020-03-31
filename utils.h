#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

void print_usage();
bool validOption(const char** optionList, size_t size, char* option);
bool setOption(char *option);
int str_length(); //new


#endif