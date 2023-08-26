#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H

#include "headers.h"

void execute_past_event(int index);
void print_recent_past_events(int* linecount);
void purge_past_events();
void pastevents(char *tokens[], int num_tokens, int* linecount);
int checkHistory();
void addStringToPastCommands(const char *newLine, int *num_lines);
void shiftStringsUp( int lineCount);

#endif