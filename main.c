#include "headers.h"

int last_char_is_ampersand = 0;             // Global variable to track if the last character is '&'
char current_directory[MAX_COMMAND_LENGTH]; // Global variable to store current directory
char home_directory[MAX_COMMAND_LENGTH];    // Global variable to store home directory
char prev_directory[MAX_COMMAND_LENGTH];    // Global variable to store prev directory
char past_commands[MAX_PAST_EVENTS][MAX_COMMAND_LENGTH];
pid_t background_processes[MAX_BACKGROUND_PROCESSES];
char background_commands[MAX_BACKGROUND_PROCESSES][MAX_COMMAND_LENGTH];
int num_background_processes = 0;

int default_input_fd;  // Store the default input file descriptor (usually 0 for stdin)
int default_output_fd; // Store the default output file descriptor (usually 1 for stdout)
int foregroundProcess = 0;
pid_t foregroundProcessPID = -1;

//  neonate
void clearInputBuffer()
{
    // while (kbhit())
    // {
    //     getchar(); // Read and discard one character at a time
    // }
    fflush(stdout);
}
void die(const char *s)
{
    perror(s);
    exit(1);
}
int keepRunning = 1;
struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}
void handleSigInt(int sig)
{
    if (sig == SIGINT)
    {
        printf("\nExiting loop...\n");
        keepRunning = 0; // Set the flag to exit the loop
    }
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

int kbhit()
{
    struct timeval tv;
    fd_set read_fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &read_fds);
}

int getMostRecentPID()
{
    FILE *fp;
    char buffer[MAX_BUFFER_SIZE];
    int pid;

    // Open the /proc/sys/kernel/ns_last_pid file for reading
    fp = fopen("/proc/sys/kernel/ns_last_pid", "r");
    if (fp == NULL)
    {
        perror("Failed to open /proc/sys/kernel/ns_last_pid");
        return -1;
    }

    // Read the PID from the file
    if (fgets(buffer, sizeof(buffer), fp) == NULL)
    {
        perror("Failed to read /proc/sys/kernel/ns_last_pid");
        fclose(fp);
        return -1;
    }

    // Close the file
    fclose(fp);

    // Convert the buffer to an integer (PID)
    pid = atoi(buffer);

    return pid;
}

//  neonate over
// Define the signal handler for Ctrl-C (SIGINT)
void sigintHandler(int signo)
{
    if (foregroundProcessPID != -1)
    {
        kill(foregroundProcessPID, SIGINT); // Send SIGINT to the foreground process
    }
}

// Define the signal handler for Ctrl-Z (SIGTSTP)
void sigtstpHandler(int signo)
{
    if (foregroundProcessPID != -1)
    {
        kill(foregroundProcessPID, SIGTSTP); // Send SIGTSTP to the foreground process
        foregroundProcess = 0;               // Set the foreground process flag to 0
    }
}

// Function to initialize the default input and output file descriptors
void initialize_default_fds()
{
    default_input_fd = dup(STDIN_FILENO);   // Store the default input (stdin) file descriptor
    default_output_fd = dup(STDOUT_FILENO); // Store the default output (stdout) file descriptor
}

// Function to reset input redirection to default (terminal input)
void reset_input_redirection()
{
    dup2(default_input_fd, STDIN_FILENO); // Redirect stdin to the default input file descriptor
}

// Function to reset output redirection to default (terminal output)

void reset_output_redirection(char *output_file, int clearFile)
{
    if (clearFile)
    {
        // Open the file in write mode to clear its content
        int clear_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (clear_fd == -1)
        {
            perror("Failed to open output file for clearing");
            exit(EXIT_FAILURE);
        }
        close(clear_fd);
    }
    else
    {
        dup2(default_output_fd, STDOUT_FILENO);
    }

    // Redirect stdout to the default output file descriptor
}

void executee(char *input, int *linecount)
{
    char *input_copy_peek = strdup(input);

    char *redir[MAX_TOKENS];
    // TOKEKNIZATION PART
    int redir_count = 0;
    tokenize_input_space(input, redir, &redir_count);
    char *commandsss[MAX_TOKENS];
    int i;
    int input_redirection = 0;
    int output_redirection = 0;
    int append_redirection = 0;
    char *input_file = NULL;
    char *output_file = NULL;
    int temp1 = 0;
    int temp2 = redir_count - 1;

    // Reset redirection-related variables before processing a new command

    // Check for redirection symbols and filenames
    for (i = 0; i < redir_count; i++)
    {
        if (strcmp(redir[i], "<") == 0)
        {
            input_redirection = 1;
            input_file = redir[i + 1];
            temp1 = i;
        }
        else if (strcmp(redir[i], ">") == 0)
        {
            output_redirection = 1;
            output_file = redir[i + 1];
            temp2 = i - 1;
        }
        else if (strcmp(redir[i], ">>") == 0)
        {
            append_redirection = 1;
            output_file = redir[i + 1];
            temp2 = i - 1;
        }
    }
    char *concatenated_string = (char *)malloc(MAX_COMMAND_LENGTH);
    strcpy(concatenated_string, ""); // Initialize it as an empty string

    if (temp1 >= 0 && temp2 < redir_count)
    {
        // Calculate the number of tokens to include in the string
        int num_tokens_to_include = temp2 - temp1 + 1;
        if (temp1 == 0)
        {
            for (int i = temp1; i <= temp2; i++)
            {
                strcat(concatenated_string, redir[i]);
                if (i < temp2)
                {
                    strcat(concatenated_string, " "); // Add a space between tokens
                }
            }
        }
        else
        {
            for (int i = 0; i < temp1; i++)
            {
                strcat(concatenated_string, redir[i]);
                if (i < temp1 - 1)
                {
                    strcat(concatenated_string, " "); // Add a space between tokens
                }
            }
        }
    }

    // Now, concatenated_string contains the desired string
    // printf("Concatenated string: %s\n", concatenated_string);
    // printf(" %d, %d, %d", input_redirection, output_redirection, append_redirection);
    // printf("rjfgwefgw");
    // Perform redirection if needed
    if (input_redirection)
    {
        // Fork a child process
        pid_t child_pid = fork();

        if (child_pid == -1)
        {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        else if (child_pid == 0) // Child process
        {
            // Open the input file
            int input_fd = open(input_file, O_RDONLY);
            if (input_fd == -1)
            {
                perror("Failed to open input file");
                exit(EXIT_FAILURE);
            }

            // Redirect stdin to the input file descriptor
            if (dup2(input_fd, STDIN_FILENO) == -1)
            {
                perror("Failed to redirect input");
                exit(EXIT_FAILURE);
            }

            // Close the input file descriptor, as it's no longer needed
            close(input_fd);

            // Execute the command using execvp
            char *tokens[MAX_TOKENS];
            int num_tokens = 0;
            tokenize_input_string(concatenated_string, tokens, &num_tokens);
            tokens[num_tokens] = NULL;

            if (execvp(tokens[0], tokens) == -1)
            {
                perror("Failed to execute command");
                exit(EXIT_FAILURE);
            }
            return;
        }
        else // Parent process
        {
            // Wait for the child process to finish
            int status;
            if (waitpid(child_pid, &status, 0) == -1)
            {
                perror("Wait for child process failed");
                exit(EXIT_FAILURE);
            }
        }
        return;
    }
    else
    {
        // If no input redirection is specified, reset to default (terminal input)
        reset_input_redirection();
    }

    if (output_redirection)
    {
        handle_append_redirection(output_file);
        // For ">" case, clear the file content and reset redirection
        FILE *file = fopen(output_file, "w");
        reset_output_redirection(output_file, 1);
    }
    else if (!output_redirection && !append_redirection)
    {
        // If no output redirection is specified, reset to default (terminal output)
        reset_output_redirection(output_file, 0);
    }
    else if (append_redirection)
    {
        handle_append_redirection(output_file);
    }
    else
    {
        // If no append redirection is specified, reset to default (terminal output)
        reset_output_redirection(output_file, 0);
    }
    input_redirection = 0;
    output_redirection = 0;
    append_redirection = 0;
    input_file = NULL;
    output_file = NULL;

    // TOKEKNIZATION PART
    int num_commands = 0;
    strcpy(input, concatenated_string);
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
    // if (strcmp(input, "exit") == 0)
    // {
    //     break;
    // }
    // printf("%d num \n", num_background_processes);
    if (result_tokens != NULL)
    {

        int m = 0;
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
                // addStringToPastCommands(input, &linecount);
                warp_command(tokens, num_tokens);
            }
            else if (strcmp(tokens[0], "peek") == 0)
            {
                // save_past_event(input);
                // addStringToPastCommands(input, &linecount);
                peek(input_copy_peek);
            }
            else if (strcmp(tokens[0], "pastevents") == 0)
            {
                pastevents(tokens, num_tokens, linecount);
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
                // printf("before\n");
                tokenize_input_space(input_seek, tokens_seek, &num_tokens_seek);
                // printf("after\n");

                // printf("%s %s", tokens_seek[0], tokens_seek[1]);
                // seek(tokens_seek[2], tokens_seek[3], tokens_seek[1]);
                seek(tokens_seek, num_tokens_seek);
            }
            else if (strcmp(tokens[0], "activities") == 0)
            {
                print_activities(&num_background_processes);
            }
            else if (strcmp(tokens[0], "ping") == 0)
            {
                pid_t pid = atoi(tokens[1]);
                int token = atoi(tokens[2]);
                ping(pid, token);
            }
            else if (strcmp(tokens[0], "fg") == 0)
            {
                pid_t pid = atoi(tokens[1]);
                fg_command(pid, &num_background_processes);
            }
            else if (strcmp(tokens[0], "bg") == 0)
            {
                pid_t pid = atoi(tokens[1]);
                bg_command(pid, &num_background_processes);
            }
            else if (strcmp(tokens[0], "iman") == 0)
            {
                man(tokens[1]);
            }
            else if (strcmp(tokens[0], "neonate") == 0)
            {
                int keepRunning = 1;
                int pidPrintInterval = atoi(tokens[2]);
                while (keepRunning)
                {
                    setbuf(stdout, NULL);
                    enableRawMode();
                    printf("Most Recently Created Process-ID: ");

                    // Get the PID of the most recently created process
                    int pid = getMostRecentPID();
                    printf("%d", pid);
                    char c;
                    int timeCounter = 0;
                    while (timeCounter < pidPrintInterval)
                    {
                        if (kbhit())
                        {
                            c = getchar();
                            timeCounter = 0;
                            if (c == 'x')
                            {
                                printf("\nExiting loop...\n");
                                keepRunning = 0; // Set the flag to exit the loop
                                break;
                            }
                        }
                        sleep(1);
                        timeCounter++;
                    }

                    disableRawMode();
                    printf("\n");
                }
            }
            else
            {
                char *com = strdup(commandsss[m]);
                char *tokens_cmd[MAX_TOKENS];
                int num_tokens_cmd = 0;
                tokenize_input_andd(com, tokens_cmd, &num_tokens_cmd);
                // printf("%d", last_char_is_ampersand);
                // num_background_processes = 0;
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
                        // for (int i = 0; i < num_tokens_cmdmm; i++)
                        // {
                        //     printf("Token %d: %s\n", i, tokens_cmdmm[i]);
                        //     // free(tokens[i]); // Free the allocated memory for each token
                        // }
                        // handle_redirection(tokens_cmdmm, &num_tokens_cmdmm);
                        // printf("pass\n");
                        execute_foreground_process(tokens_cmdmm, num_tokens_cmdmm);
                    }
                    // Check for and print ended background processes
                    print_ended_background_processes(num_tokens_cmdmm);
                }
            }
            m++;
            char full_path[256];
        }
    }
    input_redirection = 0;
    output_redirection = 0;
    append_redirection = 0;
    input_file = NULL;
    output_file = NULL;
    reset_input_redirection();
    reset_output_redirection(output_file, 0);
}

int main()
{

    int linecount = 0;
    getcwd(current_directory, sizeof(current_directory));
    getcwd(home_directory, sizeof(home_directory));
    strcpy(prev_directory, current_directory); // Update prev_directory
    // printf("%d\n heyy", linecount);
    signal(SIGINT, sigintHandler);   // Ctrl-C
    signal(SIGTSTP, sigtstpHandler); // Ctrl-Z
    linecount = checkHistory();
    // printf("%d\n heyy", linecount);
    // Read a file named .past_history if exists
    // Result - Number of History + Stored in a array
    // Keep accepting commands

    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt();
        char input[4096];
        fgets(input, 4096, stdin);
      
       if (strncmp(input, "pastevents",10) != 0)
        {
            addStringToPastCommands(input, &linecount);
        }

        input[strlen(input) - 1] = '\0'; // Remove the newline


        executee(input, &linecount);
    }
}
