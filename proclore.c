#include "proclore.h"


void print_process_info(pid_t pid) {
    char status_path[256];
    char line[256];
    sprintf(status_path, "/proc/%d/status", pid);

    FILE *status_file = fopen(status_path, "r");
    if (status_file == NULL) {
        perror("Error opening status file");
        return;
    }

    char process_status[10] = "";
    char process_group[10] = "";
    char virtual_memory[10] = "";
    char executable_path[256] = "";

    while (fgets(line, sizeof(line), status_file)) {
        if (strstr(line, "State:") == line) {
            sscanf(line, "State:%s", process_status);
        } else if (strstr(line, "PPid:") == line) {
            sscanf(line, "PPid:%s", process_group);
        } else if (strstr(line, "VmSize:") == line) {
            sscanf(line, "VmSize:%s", virtual_memory);
        }
    }

    fclose(status_file);

    if (strlen(process_status) > 0) {
        printf("pid : %d\n", pid);
        printf("process status : %s\n", process_status);
        printf("Process Group : %s\n", process_group);
        printf("Virtual memory : %s\n", virtual_memory);


        char executable_link[256];
        sprintf(executable_link, "/proc/%d/exe", pid);
        ssize_t len = readlink(executable_link, executable_path, sizeof(executable_path) - 1);
        if (len != -1) {
            executable_path[len] = '\0';
            printf("executable path : %s\n", executable_path);
        } else {
            perror("Error getting executable path");
        }
    } else {
        printf("Information not available for pid %d\n", pid);
    }
}