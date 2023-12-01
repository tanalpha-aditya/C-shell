#ifndef __SEEK_H
#define __SEEK_H

#include "headers.h"

// int match_flags(char *flags);
// void seek_recursive(const char *search, const char *current_path, int search_type);
void seek ( char *tokens[1024], int num);
void searchFile(const char *dir_path, const char *file_name, int depth, int d, int f, int e) ;

#endif