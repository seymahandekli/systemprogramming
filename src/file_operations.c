#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include "../include/file_operations.h"

int copy_file(const char *source, const char *destination) {
    int src_fd = open(source, O_RDONLY);
    if (src_fd < 0) {
        perror("Error opening source file");
        return -1;
    }

    int dest_fd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        perror("Error opening destination file");
        close(src_fd);
        return -1;
    }

    char buffer[4096];
    ssize_t bytes_read;
    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        if (write(dest_fd, buffer, bytes_read) < 0) {
            perror("Error writing to destination file");
            close(src_fd);
            close(dest_fd);
            return -1;
        }
    }

    close(src_fd);
    close(dest_fd);
    return 0;
}

int create_file(const char *file_name) {
    // Attempt to create the file
    int fd = open(file_name, O_CREAT | O_EXCL | O_WRONLY, 0644);
    if (fd == -1) {
        if (errno == EEXIST) {
            printf("File '%s' already exists.\n", file_name);
        } else {
            perror("Error creating file");
        }
        return -1; // Failure
    }

    // Close the file descriptor
    close(fd);
    printf("File '%s' created successfully.\n", file_name);
    return 0; // Success
}

int remove_file(const char *file_name) {
    // Attempt to delete the file
    if (unlink(file_name) == -1) {
        if (errno == ENOENT) {
            printf("File '%s' does not exist.\n", file_name);
        } else {
            perror("Error deleting file");
        }
        return -1; // Failure
    }

    printf("File '%s' deleted successfully.\n", file_name);
    return 0; // Success
}

int read_file(const char *file_name) {
    // Open the file for reading
    int fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file for reading");
        return -1; // Failure
    }

    // Read the file contents
    char buffer[1024];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read); // Write to stdout
    }

    // Check for read errors
    if (bytes_read == -1) {
        perror("Error reading file");
        close(fd);
        return -1; // Failure
    }

    // Close the file
    close(fd);
    printf("\n");
    return 0; // Success
}

int write_file(const char *file_name, const char *data) {
    // Open the file for writing (create if it doesn't exist, truncate if it does)
    int fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file for writing");
        return -1; // Failure
    }

    // Write the data to the file
    ssize_t bytes_written = write(fd, data, strlen(data));
    if (bytes_written == -1) {
        perror("Error writing to file");
        close(fd);
        return -1; // Failure
    }

    // Close the file
    close(fd);
    printf("Data written to '%s' successfully.\n", file_name);
    return 0; // Success
}

int move_file(const char *source_path, const char *destination_path) {
    // Attempt to move the file
    if (rename(source_path, destination_path) == -1) {
        perror("Error moving file");
        return -1; // Failure
    }

    printf("File moved from '%s' to '%s' successfully.\n", source_path, destination_path);
    return 0; // Success
}

void search_folder(const char *folder_path, const char *search_name) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf; // Now this is a complete type
    int found = 0; // Flag to track if any matches are found

    // Open the directory
    dir = opendir(folder_path);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    printf("Searching in '%s' for '%s':\n", folder_path, search_name);

    // Iterate through directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Construct the full path of the entry
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", folder_path, entry->d_name);

        // Get file/directory information
        if (stat(full_path, &statbuf) == -1) {
            perror("Error getting file info");
            continue;
        }

        // Check if the entry name matches the search name
        if (strstr(entry->d_name, search_name) != NULL) {
            printf("Found: %s (%s)\n", entry->d_name, S_ISDIR(statbuf.st_mode) ? "Directory" : "File");
            found = 1; // Set the flag to indicate a match was found
        }
    }

    // Close the directory
    closedir(dir);

    // Print a message if no matches were found
    if (!found) {
        printf("No files or directories found matching '%s'.\n", search_name);
    }
}