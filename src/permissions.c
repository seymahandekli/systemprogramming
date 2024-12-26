#include <stdio.h>
#include <sys/stat.h>
#include "permissions.h"

int change_permissions(const char *path, mode_t mode) {
    if (chmod(path, mode) < 0) {
        perror("Error changing permissions");
        return -1;
    }
    return 0;
}
