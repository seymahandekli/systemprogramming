#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <sys/types.h>

int change_permissions(const char *path, mode_t mode);

#endif
