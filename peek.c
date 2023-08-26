#include "peek.h"
int show_hidden = 0; // Default: do not show hidden files

int compare_names(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void list_directory(const char *path)
{
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    char *entries[100]; // You can adjust the size as needed
    int entry_count = 0;

    while ((entry = readdir(dir)) != NULL)
    {
        if (!show_hidden && entry->d_name[0] == '.')
        {
            continue; // Skip hidden files and directories
        }

        entries[entry_count] = strdup(entry->d_name);
        entry_count++;
    }

    closedir(dir);

    // Sort the entries
    qsort(entries, entry_count, sizeof(char *), compare_names);

    // Print the sorted entries
    for (int i = 0; i < entry_count; i++)
    {
        printf("%s\n", entries[i]);
        free(entries[i]); // Free memory allocated for each entry
    }
}

void list_detailed(const char *path)
{
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("opendir");
        return;
    }
    long total_blocks = 0; // Initialize total_blocks
    struct dirent *entry;
    char *entries[100]; // You can adjust the size as needed
    int entry_count = 0;

    while ((entry = readdir(dir)) != NULL)
    {
        if (!show_hidden && entry->d_name[0] == '.')
        {
            continue; // Skip hidden files and directories
        }

        struct stat file_stat;
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        if (stat(full_path, &file_stat) == -1)
        {
            perror("stat");
            closedir(dir);
            return;
        }
        total_blocks += file_stat.st_blocks; // Update total_blocks

        entries[entry_count] = strdup(entry->d_name);
        entry_count++;
    }

    closedir(dir);

    // Sort the entries
    qsort(entries, entry_count, sizeof(char *), compare_names);

    // Print total block count
    printf("total %ld\n", total_blocks / 2);

    // Print the sorted detailed entries
    for (int i = 0; i < entry_count; i++)
    {
        struct stat file_stat;
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entries[i]);
        if (stat(full_path, &file_stat) == -1)
        {
            perror("stat");
            return;
        }

        // Format permissions string
        char permissions[11];
        permissions[0] = (S_ISDIR(file_stat.st_mode)) ? 'd' : '-';
        permissions[1] = (file_stat.st_mode & S_IRUSR) ? 'r' : '-';
        permissions[2] = (file_stat.st_mode & S_IWUSR) ? 'w' : '-';
        permissions[3] = (file_stat.st_mode & S_IXUSR) ? 'x' : '-';
        permissions[4] = (file_stat.st_mode & S_IRGRP) ? 'r' : '-';
        permissions[5] = (file_stat.st_mode & S_IWGRP) ? 'w' : '-';
        permissions[6] = (file_stat.st_mode & S_IXGRP) ? 'x' : '-';
        permissions[7] = (file_stat.st_mode & S_IROTH) ? 'r' : '-';
        permissions[8] = (file_stat.st_mode & S_IWOTH) ? 'w' : '-';
        permissions[9] = (file_stat.st_mode & S_IXOTH) ? 'x' : '-';
        permissions[10] = '\0';

        // Format time
        char mod_time[20];
        strftime(mod_time, sizeof(mod_time), "%b %d %H:%M", localtime(&file_stat.st_mtime));

        // Print formatted information
        printf("%s %2lu %-8s %-8s %8ld %s %s\n",
               permissions,
               file_stat.st_nlink,
               getpwuid(file_stat.st_uid)->pw_name,
               getgrgid(file_stat.st_gid)->gr_name,
               (long)file_stat.st_size,
               mod_time,
               entries[i]);

        free(entries[i]); // Free memory allocated for each entry
    }
}

void peek(char *input_peek)
{
    int num_tokens = 0;
    char *tokens_p[MAX_TOKENS];
    tokenize_input(input_peek, tokens_p, &num_tokens);
    char **result_tokens = NULL; // isko apan while ke bahar kar sakte hai kya declare
    result_tokens = (char **)malloc((num_tokens + 1) * sizeof(char *));
    for (int i = 0; i < num_tokens; i++)
    {
        result_tokens[i] = strdup(tokens_p[i]);
    }
    result_tokens[num_tokens] = NULL;

    if (result_tokens != NULL)
    {
        int i = 0;
        while (result_tokens[i] != NULL)
        {
            printf("Token %d: %s\n", i, result_tokens[i]);
            i++;
        }

        int m = 0;

        // AFTER TOKENIZATION, MOVING TOWARDS COMMANDS

        while (result_tokens[m] != NULL)
        {
            char *tokens[MAX_TOKENS];
            int num_tokens = 0;

            tokenize_input_space(result_tokens[m], tokens, &num_tokens);

            peek_final(tokens, num_tokens);
            m++;
        }
    }
}

void peek_final(char *tokens[], int num_tokens)
{

    if (num_tokens == 1)
    {
        list_directory(current_directory);
    }
    else if (num_tokens == 2)
    {
        
        if ((strcmp(tokens[1], "-a") == 0))
        {
            show_hidden = 1;
            list_directory(current_directory);
        }
        else if ((strcmp(tokens[1], "-l") == 0))
        {
            show_hidden = 0;
            list_detailed(current_directory);
        }
        else if ((strcmp(tokens[1], "-al") == 0) || (strcmp(tokens[1], "-la") == 0))
        {
            show_hidden = 1;
            list_detailed(current_directory);
        }
    }
    else if( num_tokens == 3){
        if ((strcmp(tokens[1], "-a") == 0) && (strcmp(tokens[2], "-l") == 0))
        {
            show_hidden = 1;
            list_detailed(current_directory);
        }
        else if ((strcmp(tokens[1], "-l") == 0) && (strcmp(tokens[2], "-a") == 0))
        {
            show_hidden = 1;
            list_detailed(current_directory);
        }
        else if ((strcmp(tokens[1], "-a") == 0))
        {
            show_hidden = 1;
            list_directory(current_directory);
        }
        else if ((strcmp(tokens[1], "-l") == 0))
        {
            show_hidden = 0;
            list_detailed(current_directory);
        }
        else if ((strcmp(tokens[1], "-al") == 0) || (strcmp(tokens[1], "-la") == 0))
        {
            show_hidden = 1;
            list_detailed(current_directory);
        }
    }
    else if (num_tokens == 4)
    {
        if ((strcmp(tokens[1], "-a") == 0) && (strcmp(tokens[2], "-l") == 0))
        {
            show_hidden = 1;
            list_detailed(current_directory);
        }
        else if ((strcmp(tokens[1], "-l") == 0) && (strcmp(tokens[2], "-a") == 0))
        {
            show_hidden = 1;
            list_detailed(current_directory);
        }
    }
}
