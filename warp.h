#ifndef __WARP_H
#define __WARP_H

#include "headers.h"

void change_directory(const char *new_dir);
    
void warp_command(char *tokens[], int num_tokens);

#endif