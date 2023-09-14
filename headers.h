#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>
#include <sys/wait.h>
#include <pwd.h>
#include <stdbool.h>
#include <ctype.h>
#include <libgen.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>
#include <grp.h>
#include <signal.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include "prompt.h"
#include "warp.h"
#include "token.h"
#include "peek.h"
#include "pastevents.h"
#include "fgbg.h"
#include "proclore.h"
#include "seek.h"
#include "activities.h"
#include "signal.h"
#include "redir.h"
#include "fgtobg.h"
#include "iMan.h"

#define MAX_COMMAND_LENGTH 4096
#define MAX_TOKENS 64
#define MAX_PAST_EVENTS 15
#define MAX_BACKGROUND_PROCESSES 100
#define COLOR_RESET   "\x1b[0m"
#define COLOR_GREEN   "\x1b[32m" // For files
#define COLOR_BLUE    "\x1b[34m" // For directories
#define MAX_BUFFER_SIZE 1024

extern char current_directory[MAX_COMMAND_LENGTH]; // Global variable to store current directory
extern char home_directory[MAX_COMMAND_LENGTH];    // Global variable to store current directory
extern char prev_directory[MAX_COMMAND_LENGTH];    // Global variable to store current directory
extern char rel_path[MAX_COMMAND_LENGTH];
extern char past_commands[MAX_PAST_EVENTS][MAX_COMMAND_LENGTH];
extern pid_t background_processes[MAX_BACKGROUND_PROCESSES];
extern char background_commands[MAX_BACKGROUND_PROCESSES][MAX_COMMAND_LENGTH];
// int num_background_processes = 0;

// #define HISTORY_FILE ".pastevents_history"
void executee(char *input, int *linecount);
#endif
