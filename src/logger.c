#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include "logger.h"

void write_log(const char *operation, const char *status) {
    int log_fd = open("file_manager.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd < 0) {
        perror("Error opening log file");
        return;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    dprintf(log_fd, "%02d-%02d-%04d %02d:%02d:%02d - %s: %s\n",
            t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, 
            t->tm_hour, t->tm_min, t->tm_sec, operation, status);

    close(log_fd);
}
