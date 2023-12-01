#include "headers.h"
  
void generate_prompt(char *username, char *system_name, char *cwd, char *prompt)
{
    // ANSI escape code for setting text color to green
    char green_color[] = "\x1B[32m";

    // ANSI escape code for resetting text color to default
    char reset_color[] = "\x1B[0m";

    sprintf(prompt, "%s<%s@%s:%s>%s", green_color, username, system_name, cwd, reset_color);
    if (strcmp(home_directory, current_directory) == 0)
    {
        printf("%s<%s@%s:~> %s", green_color, username, system_name, reset_color);
    }
    else if(strncmp(home_directory, current_directory, strlen(home_directory)) == 0) {
        printf("%s<%s@%s:~%s> %s", green_color, username, system_name, current_directory + strlen(home_directory), reset_color);
    }
    // If not in home directory
    else
    {
        printf("%s<%s@%s:%s> %s", green_color, username, system_name, current_directory, reset_color);
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
