#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include "../include/permissions.h"

int change_permissions(const char *path, mode_t mode) {
    // Open the file or directory
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file or directory");
        return -1; // Failure
    }

    // Change the permissions using fchmod
    if (fchmod(fd, mode) == -1) {
        perror("Error changing permissions");
        close(fd);
        return -1; // Failure
    }

    // Close the file descriptor
    close(fd);
    printf("Permissions of '%s' changed successfully to %o.\n", path, mode);
    return 0; // Success
}
