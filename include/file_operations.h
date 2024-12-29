#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

int copy_file(const char *source, const char *destination);
int create_file(const char *file_name);
int remove_file(const char *file_name);
int read_file(const char *file_name);
int write_file(const char *file_name, const char *data);
int move_file(const char *source_path, const char *destination_path);
void search_folder(const char *folder_path, const char *search_name);

#endif
