#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "directory_ops.h"

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

// Listeleme fonksiyonu
void list_directory(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    struct dirent *entry;
    struct stat file_stat;
    char full_path[1024];

    printf("%-20s %-10s %-10s %-10s %-20s\n", "Name", "Type", "Size", "Permissions", "Last Modified");
    printf("--------------------------------------------------------------------------------------\n");

    while ((entry = readdir(dir)) != NULL) {
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        
        if (stat(full_path, &file_stat) == -1) {
            perror("Error retrieving file stats");
            continue;
        }

        // Dosya türü
        const char *type = S_ISDIR(file_stat.st_mode) ? "Directory" : "File";

        // Son düzenlenme tarihi
        char time_buf[20];
        struct tm *timeinfo = localtime(&file_stat.st_mtime);
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M", timeinfo);

        // Bilgileri yazdır
        printf("%-20s %-10s %-10ld ", entry->d_name, type, file_stat.st_size);
        print_permissions(file_stat.st_mode);
        printf(" %-20s\n", time_buf);
    }

    closedir(dir);
}
