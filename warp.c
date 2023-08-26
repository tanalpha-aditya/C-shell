#include "warp.h"
#include "prompt.h"

void change_directory(const char *new_dir)
{
    if (chdir(new_dir) == 0)
    {
        getcwd(current_directory, sizeof(current_directory));
    }
    else
    {
        printf("Failed to change directory to %s: %s\n", new_dir, strerror(errno));
    }
}

void warp_command(char *tokens[], int num_tokens)
{
    char *home_dir = getenv("HOME");
    if (home_dir == NULL)
    {
        printf("Home directory not found.\n");
        return;
    }

    for (int i = 1; i < num_tokens; i++)
    {
        char *arg = tokens[i];

        if (strcmp(arg, "~") == 0)
        {
            strcpy(prev_directory, current_directory); // Update prev_directory
            strcpy(current_directory, home_directory);
        }
        else if (strcmp(arg, "-") == 0)
        {
            // Logic to switch back to the previous directory
            // printf(" %s and %s\n", &prev_directory, &current_directory);
            strcpy(current_directory, prev_directory);
            // printf(" %s and %s\n", &prev_directory, &current_directory);
        }
        else if (strcmp(arg, ".") == 0)
        {
            strcpy(prev_directory, current_directory); // Update prev_directory
            strcpy(current_directory, current_directory);
        }
        else if (strcmp(arg, "..") == 0)
        {
            strcpy(prev_directory, current_directory); // Update prev_directory
            char parent_dir[MAX_COMMAND_LENGTH];
            strcpy(parent_dir, current_directory);          // Make a copy of the current directory
            strcpy(current_directory, dirname(parent_dir)); // Get the parent directory
        }
        else if (arg[0] == '/')
        {
            strcpy(prev_directory, current_directory); // Update prev_directory
            strcpy(current_directory, arg);            // Absolute path
        }
        else
        {
            strcpy(prev_directory, current_directory);                                            // Update prev_directory
            snprintf(current_directory, sizeof(current_directory), "%s/%s", prev_directory, arg); // Relative path

            if (chdir(current_directory) != 0)
            {
                printf("Directory not found: %s\n", current_directory);
                strcpy(current_directory, prev_directory); // Revert to previous directory
            }
        }
        // printf(" %s and %s\n", &prev_directory, &current_directory);
        change_directory(current_directory);
    }
}
