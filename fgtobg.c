#include "fgtobg.h"

void fg_command(int pid, int *num_background_processes)
{
    // Search for the process with the given pid in background_commands
    int found = 0;
    for (int i = 0; i < *num_background_processes; i++)
    {
        pid_t bg_pid = background_processes[i];

        if (bg_pid == pid)
        {
            // Found the process, bring it to the foreground
            int status;
            if (waitpid(pid, &status, 0) != -1)
            {
                found = 1;
                printf("[%d] finished.\n", pid);
            }
        }
    }

    if (!found)
    {
        printf("No such process found with PID %d\n", pid);
    }
}

void bg_command(int pid, int *num_background_processes)
{
    // Search for the process with the given pid in background_commands
    int found = 0;
    for (int i = 0; i < *num_background_processes; i++)
    {
        pid_t bg_pid = background_processes[i];
        if (bg_pid == pid)
        {
            // Found the process, change its state to running (send SIGCONT)
            if (kill(pid, SIGCONT) == 0)
            {
                found = 1;
                printf("[%d] resumed in the background.\n", pid);
            }
        }
    }

    if (!found)
    {
        printf("No such process found with PID %d\n", pid);
    }
}
