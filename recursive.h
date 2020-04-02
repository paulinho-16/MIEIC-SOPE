#ifndef RECURSIVE_H
#define RECURSIVE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"

#define READ 0
#define WRITE 1

void explore_file(char* path, struct dirent *direntp, struct stat *stat_buf);

void explore_directory(char* path, struct dirent *direntp, struct stat *stat_buf);

int inicial_directory(char* dirpath);

void exec_dir(char* current_path);

int recursive_tree(char* dirpath, char* fullpath);



#endif