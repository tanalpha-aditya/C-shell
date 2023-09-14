#include "fgbg.h"


void execute_foreground_process(char *tokens[], int num_tokens)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        // Child process
        execvp(tokens[0], tokens);
        // If execvp fails, print an error and exit
        perror("Command execution error");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        // Parent process
        // *current_foreground_pid = pid;
        waitpid(pid, NULL, 0); // Wait for the child process to finish
    }
    else
    {
        // Fork error
        perror("Fork error");
    }
}

void execute_background_process(char *tokens[], int num_tokens, int *num_background_processes, char *input) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execvp(tokens[0], tokens);
        // If execvp fails, print an error and exit
        perror("Command execution error");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process
        if (*num_background_processes < MAX_BACKGROUND_PROCESSES) {
            // background_processes[*num_background_processes] = pid;
            // num_background_processes++;
            add_background_process(pid, input, num_background_processes);
            printf("Background process started with PID %d\n", pid);
        } else {
            printf("Maximum number of background processes reached\n");
        }
    } else {
        // Fork error
        perror("Fork error");
    }
}


void print_ended_background_processes(int num_background_processes)
{
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        for (int i = 0; i < num_background_processes; i++) {
            if (background_processes[i] == pid) {
                printf("Background process with PID %d ended: %s\n", pid, background_commands[i]);
                if (WIFEXITED(status)) {
                    printf("Process ended normally\n");
                } else {
                    printf("Process ended abnormally\n");
                }
                // Shift remaining background processes to fill the gap
                for (int j = i; j < num_background_processes - 1; j++) {
                    background_processes[j] = background_processes[j + 1];
                    strncpy(background_commands[j], background_commands[j + 1], MAX_COMMAND_LENGTH);
                }
                num_background_processes--;
                break;
            }
        }
    }

}

void add_background_process(pid_t pid, const char *command, int *num_background_processes) {
    if (*num_background_processes < MAX_BACKGROUND_PROCESSES) {
        background_processes[*num_background_processes] = pid;
        strncpy(background_commands[*num_background_processes], command, MAX_COMMAND_LENGTH);
        (*num_background_processes)++;
    } else {
        printf("Maximum number of background processes reached\n");
    }
}