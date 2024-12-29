#include <stdio.h>
#include <string.h>
#include "file_operations.h"
#include "directory_ops.h"
#include "permissions.h"
#include "../include/logger.h"

void display_help();

// Function to parse the input command and handle quoted arguments
void parse_command(char *command, char **operation, char **arg1, char **arg2)
{
    *operation = *arg1 = *arg2 = NULL;
    char *token = command;
    int in_quotes = 0;

    while (*token)
    {
        // Skip leading spaces
        while (*token == ' ')
            token++;

        // Check if the token starts with a quote
        if (*token == '"')
        {
            in_quotes = 1;
            token++;
        }

        // Determine the end of the token
        char *end = token;
        while (*end && (in_quotes ? (*end != '"') : (*end != ' ')))
            end++;

        // Extract the token
        if (*operation == NULL)
        {
            *operation = token;
        }
        else if (*arg1 == NULL)
        {
            *arg1 = token;
        }
        else if (*arg2 == NULL)
        {
            *arg2 = token;
        }

        // Terminate the token
        if (*end)
        {
            *end = '\0';
            token = end + 1;
        }
        else
        {
            token = end;
        }

        // If we were in quotes, skip the closing quote
        if (in_quotes && *token == '"')
            token++;
        in_quotes = 0;
    }
}

int main()
{
    char command[256];
    char *operation, *arg1, *arg2;

    printf("Terminal-Based File Manager\n");
    printf("Type 'help' for a list of commands.\n");

    while (1)
    {
        printf("> ");
        if (fgets(command, sizeof(command), stdin) == NULL)
            break;

        // Remove the newline character
        command[strcspn(command, "\n")] = 0;

        // Parse the command
        parse_command(command, &operation, &arg1, &arg2);

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
        else if (strcmp(operation, "sread") == 0 && arg1)
        {
            if (read_file(arg1) == 0)
                write_log("File Read", "Success");
            else
                write_log("File Read", "Failed");
        }
        else if (strcmp(operation, "swrite") == 0 && arg1 && arg2)
        {
            if (write_file(arg1, arg2) == 0)
                write_log("File Write", "Success");
            else
                write_log("File Write", "Failed");
        }
        else if (strcmp(operation, "smove") == 0 && arg1 && arg2)
        {
            if (move_file(arg1, arg2) == 0)
                write_log("File Move", "Success");
            else
                write_log("File Move", "Failed");
        }
        else if (strcmp(operation, "screatefile") == 0 && arg1)
        {
            if (create_file(arg1) == 0)
                write_log("File Create", "Success");
            else
                write_log("File Create", "Failed");
        }
        else if (strcmp(operation, "sremovefile") == 0 && arg1)
        {
            if (remove_file(arg1) == 0)
                write_log("File Remove", "Success");
            else
                write_log("File Remove", "Failed");
        }
        else if (strcmp(operation, "screatedir") == 0 && arg1)
        {
            if (create_folder(arg1) == 0)
                write_log("Folder Create", "Success");
            else
                write_log("Folder Create", "Failed");
        }
        else if (strcmp(operation, "sremovedir") == 0 && arg1)
        {
            if (remove_folder(arg1) == 0)
                write_log("Folder Remove", "Success");
            else
                write_log("Folder Remove", "Failed");
        }
        else if (strcmp(operation, "ssearch") == 0 && arg1 && arg2)
        {
            search_folder(arg1, arg2);
        }
        else if (strcmp(operation, "schmod") == 0 && arg1 && arg2)
        {                                        // New operation: Change permissions
            mode_t mode = strtol(arg2, NULL, 8); // Convert the mode string to an octal number
            if (change_permissions(arg1, mode) == 0)
                write_log("Change Permissions", "Success");
            else
                write_log("Change Permissions", "Failed");
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
    printf("  slist <directory>        - List contents of a directory\n");
    printf("  screatedir <dirname>     - Create a new directory\n");
    printf("  sremovedir <dirname>     - Remove a directory\n");
    printf("  scopy <source> <dest>    - Copy a file from source to destination\n");
    printf("  screatefile <filename>   - Create a new file\n");
    printf("  sremovefile <filename>   - Remove a file\n");
    printf("  sread <filename>         - Read and display the contents of a file\n");
    printf("  swrite <filename> <data> - Write data to a file\n");
    printf("  smove <source> <dest>    - Move a file from source to destination\n");
    printf("  ssearch <folder> <name>  - Search for files/directories within a folder\n");
    printf("  schmod <path> <mode>     - Change permissions of a file or directory (e.g., 0644)\n");
    printf("  help                     - Display this help message\n");
    printf("  exit                     - Exit the program\n");
}
