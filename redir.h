#ifndef __REDIR_H
#define __REDIR_H

#include "headers.h"

void handle_redirection(char **tokens, int *num_tokens);
int file_exists(const char *filename) ;
void handle_input_redirection(const char *filename);
void handle_output_redirection(const char *filename);
void handle_append_redirection(const char *filename) ;

#endif