#include "headers.h"

int last_char_is_ampersand = 0;             // Global variable to track if the last character is '&'
char current_directory[MAX_COMMAND_LENGTH]; // Global variable to store current directory
char home_directory[MAX_COMMAND_LENGTH];    // Global variable to store home directory
char prev_directory[MAX_COMMAND_LENGTH];    // Global variable to store prev directory
char past_commands[MAX_PAST_EVENTS][MAX_COMMAND_LENGTH];
pid_t background_processes[MAX_BACKGROUND_PROCESSES];
char background_commands[MAX_BACKGROUND_PROCESSES][MAX_COMMAND_LENGTH];

int main()
{

    int linecount = 0;
    getcwd(current_directory, sizeof(current_directory));
    getcwd(home_directory, sizeof(home_directory));
    strcpy(prev_directory, current_directory); // Update prev_directory
    linecount = checkHistory();                // Read a file named .past_history if exists
    // Result - Number of History + Stored in a array

    // Keep accepting commands
    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt();
        char input[4096];
        fgets(input, 4096, stdin);
        input[strlen(input) - 1] = '\0'; // Remove the newline
        char *commandsss[MAX_TOKENS];

        // TOKEKNIZATION PART
        int num_commands = 0;
        tokenize_semi(input, commandsss, &num_commands);

        int last_char_is_ampersand = (input[strlen(input) - 1] == '&') ? 1 : 0; // char *input_copy_peek = strdup(commandsss[i]);
        // char *command = strdup(commandsss[i]);
        // int num_tokens = 0;
        // char *tokens[MAX_TOKENS];
        // tokenize_and(commandsss[i], tokens, &num_tokens);
        char **result_tokens = NULL; // isko apan while ke bahar kar sakte hai kya declare
        result_tokens = (char **)malloc((num_commands + 1) * sizeof(char *));
        for (int i = 0; i < num_commands; i++)
        {
            result_tokens[i] = strdup(commandsss[i]);
        }
        result_tokens[num_commands] = NULL;
        if (strcmp(input, "exit") == 0)
        {
            break;
        }

        if (result_tokens != NULL)
        {

            int m = 0;

            int num_background_processes = 0;

            while (result_tokens[m] != NULL)
            {
                char *input_copy_peek = strdup(result_tokens[m]);
                char *input_seek = strdup(result_tokens[m]);
                char *tokens[MAX_TOKENS];
                int num_tokens = 0;
                tokenize_input_string(result_tokens[m], tokens, &num_tokens);

                // for (int i = 0; i < num_tokens; i++)
                // {
                //     printf("Token %d: %s\n", i, tokens[i]);
                //     // free(tokens[i]); // Free the allocated memory for each token
                // }

                if (strcmp(tokens[0], "warp") == 0)
                {
                    // save_past_event(input);
                    addStringToPastCommands(input, &linecount);
                    warp_command(tokens, num_tokens);
                }
                else if (strcmp(tokens[0], "peek") == 0)
                {
                    // save_past_event(input);
                    addStringToPastCommands(input, &linecount);
                    peek(input_copy_peek);
                }
                else if (strcmp(tokens[0], "pastevents") == 0)
                {
                    pastevents(tokens, num_tokens, &linecount);
                }
                else if (strcmp(tokens[0], "proclore") == 0)
                {
                    if (num_tokens == 1)
                    {
                        print_process_info(getpid());
                    }
                    else
                    {
                        pid_t pid = atoi(tokens[1]);
                        print_process_info(pid);
                    }
                }
                else if (strcmp(tokens[0], "seek") == 0)
                {
                    char *tokens_seek[MAX_TOKENS];
                    int num_tokens_seek = 0;
                    tokenize_input_space(input_seek, tokens_seek, &num_tokens_seek);
                    printf("%s", tokens_seek[1]);
                    seek(tokens_seek[2], tokens_seek[3], tokens_seek[1]);
                }
                else
                {
                    char *com = strdup(commandsss[m]);
                    char *tokens_cmd[MAX_TOKENS];
                    int num_tokens_cmd = 0;
                    tokenize_input_andd(com, tokens_cmd, &num_tokens_cmd);
                    // printf("%d", last_char_is_ampersand);
                    num_background_processes = 0;
                    for (int b = 0; b < num_tokens_cmd; b++)
                    {
                        // printf("%d\n", num_tokens_cmd);

                        // printf("%s\n", tokens_cmd[b]);
                        char *commm = strdup(tokens_cmd[b]);
                        char *tokens_cmdmm[MAX_TOKENS];
                        int num_tokens_cmdmm = 0;
                        tokenize_input_space(commm, tokens_cmdmm, &num_tokens_cmdmm);
                        if ((num_tokens_cmd == (b + 1) && last_char_is_ampersand == 1) || (b < num_tokens_cmd - 1))
                        {
                            // Background process
                            // printf("bg\n");
                            tokens_cmd[num_tokens_cmdmm - 1] = NULL; // Remove the "&" token
                            // printf( " %d  num", num_tokens_cmdmm);
                            execute_background_process(tokens_cmdmm, num_tokens_cmdmm, &num_background_processes, commm);
                        }
                        else
                        {
                            // Foreground process
                            // printf("fg\n");
                            execute_foreground_process(tokens_cmdmm, num_tokens_cmdmm);
                        }
                        // Check for and print ended background processes
                        print_ended_background_processes(num_tokens_cmdmm);
                    }
                }
                m++;
                // printf("%s\n%s\n",tokens, tokens[0]);
                char full_path[256];

                // sprintf(full_path, "%s/%s", home_directory, ".bg");
                // FILE *temp_file = fopen(full_path, "r");
                // if (temp_file)
                // {
                //     char line[MAX_COMMAND_LENGTH];
                //     while (fgets(line, sizeof(line), temp_file))
                //     {
                //         line[strlen(line) - 1] = '\0'; // Remove newline
                //         printf("%s\n", line);
                //     }
                //     fclose(temp_file);
                //     remove(full_path); // Remove the temporary file
                // }
            }
        }
    }
}
