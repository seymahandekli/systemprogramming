#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "file_operations.h"

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
int remove_file(const char *path) {
    if (unlink(path) < 0) {
        perror("Error removing file");
        return -1;
    }
    return 0;
}
