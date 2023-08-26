#ifndef __PEEK_H
#define __PEEK_H

#include "headers.h"

int compare_names(const void *a, const void *b);
void list_directory(const char *path) ;
void list_detailed(const char *path);
void peek(char *input);
void peek_final(char *tokens[], int num_tokens);

#endif