#ifndef __PROMPT_H
#define __PROMPT_H
#include "headers.h"

void prompt();
void generate_prompt(char *username, char *system_name, char *cwd, char *prompt);

#endif