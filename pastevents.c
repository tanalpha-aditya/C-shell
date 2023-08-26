#include "pastevents.h"


void execute_past_event(int index)
{
    char full_path[256];

    sprintf(full_path, "%s/%s", home_directory, ".history");
    FILE *file = fopen(full_path, "r");
    if (file == NULL)
    {
        perror("Error reading history");
        return;
    }

    char line[MAX_COMMAND_LENGTH];
    char original_command[MAX_COMMAND_LENGTH]; // Store the original command
    int count = 0;
    while (fgets(line, sizeof(line), file))
    {
        if (count == index)
        {
            strncpy(original_command, line, sizeof(original_command) - 1);
            original_command[sizeof(original_command) - 1] = '\0';
            break;
        }
        count++;
    }

    fclose(file);

    // Execute the original command
    if (strlen(original_command) > 0)
    {
        // Process the original command
        // For example, tokenize and execute as you do in your main loop
        char *tokens[MAX_TOKENS];
        int num_tokens = 0;
        tokenize_input(original_command, tokens, &num_tokens);

        // ... your command processing code here
        // ...

        // Print the result or prompt again
        // ...
    }
    else
    {
        printf("Command at index %d not found in history.\n", index);
    }
}

void purge_past_events(int *num_lines)
{
    char full_path[256];

    sprintf(full_path, "%s/%s", home_directory, ".history");

    *num_lines = 0;
    remove(full_path);
    printf("Past events cleared.\n");
}

void print_recent_past_events(int* linecount)
{
    for (int i = 0; i < *linecount; i++)
    {
        printf("%d: %s\n", i, past_commands[i]);
    }
}

void pastevents(char *tokens[], int num_tokens, int* linecount)
{
    if (num_tokens == 1)
    {
        print_recent_past_events(linecount);
    }
    else if (num_tokens == 2)
    {
        if ((strcmp(tokens[1], "purge") == 0))
        {
            purge_past_events(linecount);
        }
    }
    else if (num_tokens == 3)
    {
        if ((strcmp(tokens[1], "execute") == 0))
        {
            int index = atoi(tokens[2]);
            execute_past_event(index);
        }
    }
}

int checkHistory()
{
    char full_path[256];

    sprintf(full_path, "%s/%s", home_directory, ".history");
    int linecount = 0; 
    FILE *file = fopen(full_path, "r"); // Replace "example.txt" with your file's name
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }


    // Read lines from the file
    while (linecount < MAX_PAST_EVENTS && fgets(past_commands[linecount], MAX_COMMAND_LENGTH, file) != NULL)
    {
        // Remove newline character if present
        char *newline = strchr(past_commands[linecount], '\n');
        if (newline)
            *newline = '\0';

        linecount++;
    }

    fclose(file);
    return linecount;
    // Print the stored lines
    // for (int i = 0; i < lineCount; i++)
    // {
    //     printf("Line %d: %s\n", i + 1, lines[i]);
    // }
}

void shiftStringsUp( int lineCount) {
    for (int i = 0; i < lineCount - 1; i++) {
        strcpy(past_commands[i], past_commands[i + 1]);
    }
}

void addStringToPastCommands(const char *newLine, int *num_lines) {

    char full_path[256];

    sprintf(full_path, "%s/%s", home_directory, ".history");

    if (*num_lines < MAX_PAST_EVENTS) {
        strcpy(past_commands[*num_lines], newLine);
        (*num_lines)++;
    } else {
        shiftStringsUp(*num_lines);
        strcpy(past_commands[MAX_PAST_EVENTS - 1], newLine);
    }


    // Delete the file if it exists
    if (remove(full_path) == 0) {
        // printf("Deleted %s\n", full_path);
    } else {
        perror("Error deleting file");
    }

    // Create the file again for writing
    FILE *file = fopen(full_path, "w");
    if (file == NULL) {
        perror("Error creating file");
        return;
    }
    // Write the array of strings into the file
    for (int i = 0; i < *num_lines; i++) {
        fprintf(file, "%s\n", past_commands[i]);

        // printf("%s \n", past_commands[i]);
    }

    fclose(file);
}