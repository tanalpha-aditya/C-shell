#include "headers.h"

// void get_relative_path(char *cwd, char *home_dir, char *rel_path)
// {
//     char rel
//     if (strncmp(cwd, home_dir, strlen(home_dir)) == 0)
//     {
//         if (strlen(cwd) >= strlen(home_dir))
//         {s
//             strcpy(rel_path, "~");

            
//         }
//     }
//     else
//     {
//         strcpy(rel_path, cwd);
//     }
// }

void generate_prompt(char *username, char *system_name, char *cwd, char *prompt)
{
    // char rel_path[MAX_COMMAND_LENGTH];
    // void get_relative_path(cwd, home_directory, rel_path);
    sprintf(prompt, "<%s@%s:%s>", username, system_name, cwd);
    if (strcmp(home_directory, current_directory) == 0)
    {
        printf("<%s@%s:~> ", username, system_name);
    }
    else if(strncmp(home_directory, current_directory, strlen(home_directory)) == 0) {
        printf("<%s@%s:~%s> ", username, system_name, current_directory + strlen(home_directory));
    }
    // If i am not in my home directory
    else
    {
        printf("<%s@%s:%s> ", username, system_name, current_directory);
    }
}   


void prompt()
{

    char system_name[MAX_COMMAND_LENGTH];
    char username[MAX_COMMAND_LENGTH];
    gethostname(system_name, sizeof(system_name));
    struct passwd *pw = getpwuid(getuid());
    strcpy(username, pw->pw_name);

    char prompt[MAX_COMMAND_LENGTH];
    generate_prompt(username, system_name, current_directory, prompt);

}
