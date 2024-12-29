#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

void write_log(const char *operation, const char *status) {
    // Open the log file in append mode
    int log_fd = open("log/file_manager.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd < 0) {
        perror("Error opening log file");
        return;
    }

    // Get the current time
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // Create a formatted log entry
    char log_entry[1024]; // Buffer size increased to 1024 bytes
    snprintf(log_entry, sizeof(log_entry),
             "╔═══════════════════════════════════════════════════════════════════════════════╗\n"
             "║ Operation: %-66s ║\n"
             "║ Status:    %-66s ║\n"
             "║ Timestamp: %02d-%02d-%04d %02d:%02d:%02d%47s ║\n"
             "╚═══════════════════════════════════════════════════════════════════════════════╝\n\n",
             operation, status, t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
             t->tm_hour, t->tm_min, t->tm_sec, "");

    // Write the log entry to the file
    write(log_fd, log_entry, strlen(log_entry));

    // Close the log file
    close(log_fd);
}