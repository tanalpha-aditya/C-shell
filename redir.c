#include "redir.h"

void handle_redirection(char **tokens, int *num_tokens) {
    int input_fd = -1;  // File descriptor for input
    int output_fd = -1; // File descriptor for output
    int append = 0;     // Flag for append mode (> or >>)

    for (int i = 0; i < *num_tokens; i++) {
        if (strcmp(tokens[i], "<") == 0) {
            // Input redirection
            if (i + 1 < *num_tokens) {
                input_fd = open(tokens[i + 1], O_RDONLY);
                if (input_fd == -1) {
                    perror("Error opening input file");
                    return;
                }
                // Remove the input redirection tokens from the command
                for (int j = i; j < *num_tokens - 2; j++) {
                    tokens[j] = tokens[j + 2];
                }
                *num_tokens -= 2;
                i -= 2; // Recheck for more redirection symbols
            } else {
                printf("No input file specified!\n");
                return;
            }
        } else if (strcmp(tokens[i], ">") == 0) {
            // Output redirection (overwrite mode)
            append = 0;
            if (i + 1 < *num_tokens) {
                output_fd = open(tokens[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (output_fd == -1) {
                    perror("Error opening output file");
                    return;
                }
                // Remove the output redirection tokens from the command
                for (int j = i; j < *num_tokens - 2; j++) {
                    tokens[j] = tokens[j + 2];
                }
                *num_tokens -= 2;
                i -= 2; // Recheck for more redirection symbols
            } else {
                printf("No output file specified!\n");
                return;
            }
        } else if (strcmp(tokens[i], ">>") == 0) {
            // Output redirection (append mode)
            append = 1;
            if (i + 1 < *num_tokens) {
                output_fd = open(tokens[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (output_fd == -1) {
                    perror("Error opening output file");
                    return;
                }
                // Remove the output redirection tokens from the command
                for (int j = i; j < *num_tokens - 2; j++) {
                    tokens[j] = tokens[j + 2];
                }
                *num_tokens -= 2;
                i -= 2; // Recheck for more redirection symbols
            } else {
                printf("No output file specified!\n");
                return;
            }
        }
    }

    // Perform the redirection
    if (input_fd != -1) {
        // Redirect input
        if (dup2(input_fd, STDIN_FILENO) == -1) {
            perror("Error redirecting input");
            return;
        }
        close(input_fd);
    }

    if (output_fd != -1) {
        // Redirect output
        if (dup2(output_fd, STDOUT_FILENO) == -1) {
            perror("Error redirecting output");
            return;
        }
        close(output_fd);
    }

    // Execute the command (after redirection)
    execvp(tokens[0], tokens);
    perror("Error executing command");
    exit(EXIT_FAILURE);
}



// Function to check if a file exists
int file_exists(const char *filename) {
    return access(filename, F_OK) != -1;
}

// Function to handle input redirection
void handle_input_redirection(const char *filename) {
    if (!file_exists(filename)) {
        printf("No such input file found!\n");
        return;
    }
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    if (dup2(fd, STDIN_FILENO) == -1) {
        perror("dup2");
        exit(1);
    }
    close(fd);
}

// Function to handle output redirection (overwrites the file)
void handle_output_redirection(const char *filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        exit(1);
    }
    close(fd);
}

// Function to handle output redirection (appends to the file)
void handle_append_redirection(const char *filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        exit(1);
    }
    close(fd);
}
