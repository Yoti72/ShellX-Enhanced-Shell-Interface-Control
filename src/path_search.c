// path_search.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "path_search.h"

char *find_command_in_path(const char *command)
{
    // If command contains a '/', it's assumed to be a path.
    if (strchr(command, '/') != NULL)
    {
        return access(command, X_OK) == 0 ? strdup(command) : NULL;
    }

    char *path = getenv("PATH");
    if (!path)
    {
        return NULL; // PATH is not set.
    }

    char *path_copy = strdup(path); // Duplicate PATH for strtok.
    char *directory = strtok(path_copy, ":");
    char *full_path = NULL;

    while (directory)
    {
        full_path = malloc(strlen(directory) + strlen(command) + 2); // +2 for '/' and '\0'.
        if (!full_path)
        {
            free(path_copy);
            return NULL;
        }

        sprintf(full_path, "%s/%s", directory, command);
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return full_path; // Found executable command.
        }

        free(full_path);
        directory = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL; // Command not foundF
}

void execute_command(tokenlist *tokens)
{
    char *command_path = find_command_in_path(tokens->items[0]);
    if (command_path)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            // Convert the tokens to an array format suitable for execv
            char **argv = malloc((tokens->size + 1) * sizeof(char *));
            for (int i = 0; i < tokens->size; i++)
            {
                argv[i] = tokens->items[i];
            }
            argv[tokens->size] = NULL;

            // In the child process, execute the command
            execv(command_path, argv);
            perror("execv"); // If execv returns, an error occurred
            exit(EXIT_FAILURE);
        }
        else if (pid > 0)
        {
            // In the parent process, wait for the child to finish
            int status;
            waitpid(pid, &status, 0);
        }
        else
        {
            perror("fork"); // Fork failed
        }
        free(command_path);
    }
    else
    {
        printf("%s: command not found\n", tokens->items[0]);
    }
}
