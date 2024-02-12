#include "IO_redirect.h"

void IO_compare(tokenlist *tokens)
{
    char *input_file = NULL;
    char *output_file = NULL;

    // Iterate over tokens to find and set input and output files
    for (int i = 0; i < tokens->size; i++)
    {
        if (strcmp(tokens->items[i], ">") == 0 && i + 1 < tokens->size)
        {
            output_file = tokens->items[i + 1]; // Set output file
            tokens->items[i] = NULL;            // Terminate args array before the redirection symbol
        }
        else if (strcmp(tokens->items[i], "<") == 0 && i + 1 < tokens->size)
        {
            input_file = tokens->items[i + 1]; // Set input file
            tokens->items[i] = NULL;           // Terminate args array before the redirection symbol
        }
    }

    // Call IO_redirect with identified input and output files
    IO_redirect(tokens->items, input_file, output_file);
}

void IO_redirect(char **args, char *input_file, char *output_file)
{
    char *command = find_command_in_path(args[0]);
    if (!command)
    {
        fprintf(stderr, "Command not found: %s\n", args[0]);
        return; // Exit the function if command not found
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    { // Child process
        // Input redirection
        if (input_file != NULL)
        {
            int fd_in = open(input_file, O_RDONLY);
            if (fd_in == -1)
            {
                perror("open for input");
                exit(EXIT_FAILURE);
            }
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }

        // Output redirection
        if (output_file != NULL)
        {
            int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_out == -1)
            {
                perror("open for output");
                exit(EXIT_FAILURE);
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }

        execv(command, args);
        perror("execv");
        exit(EXIT_FAILURE);
    }
    else
    { // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
    free(command);
}
