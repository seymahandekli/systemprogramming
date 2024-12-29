#include <stdio.h>
#include <dirent.h>
#include <stdlib.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include "../include/directory_ops.h"

// İzinleri okunabilir formatta döndürmek için yardımcı bir fonksiyon
void print_permissions(mode_t mode) {
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

// Listing function
void list_directory(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    struct dirent *entry;
    struct stat file_stat;
    char full_path[1024];

    // Header
    printf("%-30s %-10s %-10s %-12s  %-20s\n", "Name", "Type", "Size", "Permissions", "Last Modified");
    printf("------------------------------------------------------------------------------------------\n");

    while ((entry = readdir(dir)) != NULL) {
        // Skip . and .. entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construct full path
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        // Retrieve file statistics
        if (stat(full_path, &file_stat) == -1) {
            perror("Error retrieving file stats");
            continue;
        }

        // Determine file type
        const char *type = S_ISDIR(file_stat.st_mode) ? "Directory" : "File";

        // Get last modification time
        char time_buf[20];
        struct tm *timeinfo = localtime(&file_stat.st_mtime);
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M", timeinfo);

        // Print file details
        printf("%-30s %-10s %-10ld ", entry->d_name, type, file_stat.st_size);
        print_permissions(file_stat.st_mode);
        printf("    %-20s\n", time_buf);
    }

    closedir(dir);
}

int create_folder(const char *folder_name) {
    // Check if the folder already exists
    struct stat st;
    if (stat(folder_name, &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            printf("Folder '%s' already exists.\n", folder_name);
            return -1; // Folder already exists
        } else {
            printf("'%s' exists but is not a directory.\n", folder_name);
            return -1; // Path exists but is not a directory
        }
    }

    // Use the system command to create the directory
    char command[256];
    snprintf(command, sizeof(command), "mkdir %s", folder_name);
    int result = system(command);

    if (result == 0) {
        printf("Folder '%s' created successfully.\n", folder_name);
        return 0; // Success
    } else {
        perror("Error creating folder");
        return -1; // Failure
    }
}

int remove_folder(const char *folder_name) {
    // Check if the folder exists
    struct stat st;
    if (stat(folder_name, &st) == -1) {
        if (errno == ENOENT) {
            printf("Folder '%s' does not exist.\n", folder_name);
        } else {
            perror("Error checking folder");
        }
        return -1; // Failure
    }

    // Ensure it is a directory
    if (!S_ISDIR(st.st_mode)) {
        printf("'%s' is not a directory.\n", folder_name);
        return -1; // Not a directory
    }

    // Use the system command to delete the directory
    char command[256];
    snprintf(command, sizeof(command), "rmdir %s", folder_name);
    int result = system(command);

    if (result == 0) {
        printf("Folder '%s' deleted successfully.\n", folder_name);
        return 0; // Success
    } else {
        if (errno == ENOTEMPTY) {
            printf("Folder '%s' is not empty.\n", folder_name);
        } else {
            perror("Error deleting folder");
        }
        return -1; // Failure
    }
}
