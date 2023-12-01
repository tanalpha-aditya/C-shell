#include "seek.h"


void searchFile(const char *dir_path, const char *file_name, int depth, int d, int f, int e) {
    struct dirent *entry;
    DIR *dp = opendir(dir_path);

    if (dp == NULL) {
        perror("opendir");
        return;
    }

    chdir(dir_path); // Change to the specified directory
    int match_found = 0; // Flag to check if any match is found

    while ((entry = readdir(dp))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; // Skip "." and ".." directories
        }

        struct stat statbuf;
        if (stat(entry->d_name, &statbuf) == -1) {
            perror("stat");
            continue;
        }
        char last_seek_path[MAX_COMMAND_LENGTH];
        int found_ct = 0;

        if (S_ISDIR(statbuf.st_mode)) {
            // If it's a directory, recursively search it (limit depth to avoid infinite loops)
            if (depth > 0) {
                char sub_dir[MAX_COMMAND_LENGTH];
                snprintf(sub_dir, sizeof(sub_dir), "%s/%s", dir_path, entry->d_name);
                if ((d == 1 && strcmp(entry->d_name, file_name) == 0) ) {
                    printf("%s/%s\n", dir_path, entry->d_name);
                    found_ct += 1;
                    strcpy(last_seek_path, dir_path);
                }
                searchFile(sub_dir, file_name, depth - 1, d, f, e);
            }
        } else if (S_ISREG(statbuf.st_mode)) {
            // If it's a regular file, check if the name matches
            if ((strcmp(entry->d_name, file_name) == 0) ) {
                // Print the complete path to the matching file
                if (f == 1 && strcmp(entry->d_name, file_name) == 0) {
                    printf("%s/%s\n", dir_path, entry->d_name);
                    found_ct += 1;
                    strcpy(last_seek_path, dir_path);
                }
                match_found = 1;
            }
        }

        if ( f == 0 && d == 0){
            if (S_ISDIR(statbuf.st_mode)) {
            // If it's a directory, recursively search it (limit depth to avoid infinite loops)
            if (depth > 0) {
                char sub_dir[MAX_COMMAND_LENGTH];
                snprintf(sub_dir, sizeof(sub_dir), "%s/%s", dir_path, entry->d_name);
                if ((strcmp(entry->d_name, file_name) == 0) ) {
                    printf("%s/%s\n", dir_path, entry->d_name);
                    found_ct += 1;
                    strcpy(last_seek_path, dir_path);
                }
                searchFile(sub_dir, file_name, depth - 1, d, f, e);
            }
        } else if (S_ISREG(statbuf.st_mode)) {
            // If it's a regular file, check if the name matches
            if ((strcmp(entry->d_name, file_name) == 0) ) {
                // Print the complete path to the matching file
                if (strcmp(entry->d_name, file_name) == 0) {
                    printf("%s/%s\n", dir_path, entry->d_name);
                    found_ct += 1;
                    strcpy(last_seek_path, dir_path);
                }
                match_found = 1;
            }
        }
        }
    }

    chdir(".."); // Move back to the parent directory
    closedir(dp);

    if (!match_found && depth == 0) {
        printf("No match found!\n");
    }
}

void seek(char *tokens[], int num)
{
    int flagd = 0;
    int flagf = 0;
    int flage = 0;
    char *file_name = (char *)malloc(MAX_COMMAND_LENGTH);
    char *dir = (char *)malloc(MAX_COMMAND_LENGTH);

    if (num == 2)
    {
        strcpy(file_name, tokens[1]);
        strcpy(dir, current_directory);
    }
    else if (num == 3)
    {
        if (strcmp(tokens[1], "-d") == 0)
        {
            flagd = 1;
        }
        else if (strcmp(tokens[1], "-f") == 0)
        {
            flagf = 1;
        }
        else if (strcmp(tokens[1], "-e") == 0)
        {
            flage = 1;
        }
        if (flagd == flage && flage == flagf && flagf == 0)
        {
            strcpy(file_name, tokens[1]);
            strcpy(dir, current_directory);
            strcat(dir, "/");
            strcat(dir, tokens[2]);
        }
        else
        {
            strcpy(file_name, tokens[2]);

            strcpy(dir, current_directory);
        }
    }
    else if (num == 4)
    {
        if (strcmp(tokens[1], "-d") == 0)
        {
            flagd = 1;
        }
        else if (strcmp(tokens[1], "-f") == 0)
        {
            flagf = 1;
        }
        else if (strcmp(tokens[1], "-e") == 0)
        {
            flage = 1;
        }
        if (flage == 1 || flagd == 1)
        {
            if (strcmp(tokens[2], "-d") == 0)
            {
                flagd = 1;
            }
            else if (strcmp(tokens[2], "-e") == 0)
            {
                flage = 1;
            }
        }
        if (flage == 1 && flagd == 1)
        {
            strcpy(file_name, tokens[3]);
            strcpy(dir, current_directory);
        }
        else
        {
            strcpy(file_name, tokens[2]);
            strcpy(dir, current_directory);
            strcat(dir, "/");
            strcat(dir, tokens[3]);
        }
    }
    else if (num == 5)
    {
        flagd = 1;
        flage = 1;
        strcpy(file_name, tokens[3]);
        strcpy(dir, current_directory);
        strcat(dir, "/");
        strcat(dir, tokens[4]);
    }

    // printf("%d %d %d\n", flagd, flage, flagf);
    // printf("%s file\n", file_name);

    char resolved_path[1024];
    realpath(dir, resolved_path);
    // printf("%s dir\n", resolved_path);
    searchFile(resolved_path, file_name, 10, flagd, flagf, flage);
    free(file_name); // Don't forget to free the allocated memory
    free(dir);       // Don't forget to free the allocated memory
}
