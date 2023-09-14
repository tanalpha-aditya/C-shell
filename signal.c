#include "signal.h"


void ping(int pid, int signal_number)
{
    
    // Check if the process with the given PID exists
    int result = kill(pid, 0);

    if (result == 0)
    {
        // Process with the given PID exists
        // Calculate the actual signal number (taking modulo 32)
        int actual_signal = signal_number % 32;

        // Send the signal to the process
        if (kill(pid, actual_signal) == 0)
        {
            printf("Sent signal %d to process with pid %d\n", actual_signal, pid);
        }
        else
        {
            perror("Error sending signal");
        }
    }
    else if (errno == ESRCH)
    {
        // No such process found
        printf("No such process found with PID %d\n", pid);
    }
    else
    {
        perror("Error checking process existence");
    }
}
