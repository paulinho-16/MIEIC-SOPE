#ifndef UTILSQ1_H
#define UTILSQ1_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

void print_usage();
bool validOption(const char** optionList, size_t size, char* option);
bool get_input(int argc, char* argv[]);

#endif