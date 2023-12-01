#include "activities.h"

void print_activities(int *num_background_processes) {
    printf("Activities:\n");

    for (int i = 0; i < *num_background_processes; i++) {
        pid_t pid = background_processes[i];
        const char *command = background_commands[i];

        // Check if the process is still running or stopped
        int status;
        pid_t result = waitpid(pid, &status, WNOHANG);

        if (result == 0) {
            // Process is still running
            printf("[%d] : %s - Running\n", pid, command);
        } else if (result == pid) {
            // Process has ended
            if (WIFEXITED(status)) {
                printf("[%d] : %s - Stopped (Exit Status: %d)\n", pid, command, WEXITSTATUS(status));
            } else {
                printf("[%d] : %s - Stopped (Abnormal Exit)\n", pid, command);
            }
        } else {
            // An error occurred
            perror("waitpid");
        }
    }
}