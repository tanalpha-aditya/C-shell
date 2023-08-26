#include "seek.h"

int match_flags(char *flags) {
    int d_flag = 0;
    int f_flag = 0;

    for (int i = 0; flags[i] != '\0'; i++) {
        if (flags[i] == '-') {
            continue;
        } else if (flags[i] == 'd') {
            d_flag = 1;
        } else if (flags[i] == 'f') {
            f_flag = 1;
        } else {
            return -1; // Invalid flag
        }
    }

    if (d_flag && f_flag) {
        return -1; // Invalid flag combination
    }

    if (d_flag) {
        return 1;
    } else if (f_flag) {
        return 2;
    } else {
        return 0; // No specific flag
    }
}

void seek_recursive(const char *search, const char *current_path, int search_type) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(current_path);
    if (!dir) {
        perror("Failed to open directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[MAX_COMMAND_LENGTH];
        snprintf(full_path, sizeof(full_path), "%s/%s", current_path, entry->d_name);

        struct stat statbuf;
        if (stat(full_path, &statbuf) == -1) {
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            if (search_type == 0 || search_type == 1) {
                if (strcmp(entry->d_name, search) == 0) {
                    printf("%s\n", full_path);
                }
            }
            seek_recursive(search, full_path, search_type);
        } else if (S_ISREG(statbuf.st_mode)) {
            if (search_type == 0 || search_type == 2) {
                if (strcmp(entry->d_name, search) == 0) {
                    printf("%s\n", full_path);
                }
            }
        }
    }

    closedir(dir);
}

void seek(const char *search, const char *target_directory, const char *flags) {
    int search_type = 0; // 0: Any, 1: Directories, 2: Files

    if (flags[0] != '\0') {
        int flag_result = match_flags(flags);
        if (flag_result == -1) {
            printf("Invalid flags!\n");
            return;
        } else if (flag_result == 1) {
            search_type = 1; // Directories only
        } else if (flag_result == 2) {
            search_type = 2; // Files only
        }
    }

    seek_recursive(search, target_directory, search_type);
}