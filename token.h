#ifndef __TOKEN_H
#define __TOKEN_H

#include "headers.h"

bool is_inside_quotes(char *input, char quote, int pos);
void remove_extra_spaces(char *token);
void tokenize_semi(char *input, char *tokens[], int *num_tokens);
void tokenize_and(char *input, char *tokens[], int *num_tokens);
void tokenize_input_string(char *input, char *tokens[], int *num_tokens);
void tokenize_input_space(char *input, char *tokens[], int *num_tokens);
void tokenize_input(char *input, char *tokens[], int *num_tokens);
void tokenize_input_andd(const char *input, char *tokens[], int *num_tokens);
#endif