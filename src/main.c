#include <stdio.h>
#include <string.h>
#include "file_operations.h"
#include "directory_ops.h"
#include "permissions.h"
#include "logger.h"

void display_help();

int main()
{
    char command[256];
    char* operation, * arg1, * arg2;

    printf("Terminal-Based File Manager\n");
    printf("Type 'help' for a list of commands.\n");

    while (1)
    {
        printf("> ");
        if (fgets(command, sizeof(command), stdin) == NULL)
            break;

        command[strcspn(command, "\n")] = 0;
        operation = strtok(command, " ");
        arg1 = strtok(NULL, " ");
        arg2 = strtok(NULL, " ");

        if (operation == NULL)
            continue;

        if (strcmp(operation, "slist") == 0 && arg1)
        {
            list_directory(arg1);
        }
        else if (strcmp(operation, "scopy") == 0 && arg1 && arg2)
        {
            if (copy_file(arg1, arg2) == 0)
                write_log("File Copy", "Success");
            else
                write_log("File Copy", "Failed");
        }
        else if (strcmp(operation, "sremove") == 0 && arg1)
        {
            if (remove_file(arg1) == 0)
                write_log("File Remove", "Success");
            else
                write_log("File Remove", "Failed");
        }
        else if (strcmp(operation, "smkdir") == 0 && arg1)
        { // Create directory
            create_folder(arg1);
            write_log("Directory Create", "Attempted");
        }
        else if (strcmp(operation, "srmdir") == 0 && arg1)
        { // Remove directory
            delete_folder(arg1);
            write_log("Directory Remove", "Attempted");
        }
        else if (strcmp(operation, "help") == 0)
        {
            display_help();
        }
        else if (strcmp(operation, "exit") == 0)
        {
            printf("Exiting. Goodbye!\n");
            break;
        }
        else
        {
            printf("Unknown command. Type 'help' for usage.\n");
        }
    }

    return 0;
}

void display_help()
{
    printf("Available commands:\n");
    printf("  slist <directory_path>       - List files in the directory\n");
    printf("  scopy <source> <destination> - Copy a file\n");
    printf("  sremove <file_path>          - Remove a file\n");
    printf("  smkdir <folder_name>         - Create a folder\n");
    printf("  srmdir <folder_name>         - Remove a folder\n");
    printf("  help                         - Show this help\n");
    printf("  exit                         - Exit the program\n");
}
