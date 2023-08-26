#ifndef __FGBG_H
#define __FGBG_H

#include "headers.h"

void execute_foreground_process(char *tokens[], int num_tokens);
// void execute_background_process(char *tokens[], int num_tokens,int *num_background_processes);
// void print_ended_background_processes(int num_background_processes);
// void execute_background_process(char *tokens[], int num_tokens, const char *input);
void print_ended_background_processes(int num_background_processes);
void execute_background_process(char *tokens[], int num_tokens, int *num_background_processes, char *input) ;
void add_background_process(pid_t pid, const char *command, int *num_background_processes) ;

#endif