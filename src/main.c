#include "lexer.h"
#include "prompt.h"
#include "env_expansion.h"
#include "IO_redirect.h"
#include "piping.h"
#include "path_search.h"
#include "backgroundPro.h"
#include "builtin-exec.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*------------------------- PIPING ----------------------------------------*/
int contains_pipe(tokenlist *tokens)
{
    for (int i = 0; i < tokens->size; i++)
    {
        if (strcmp(tokens->items[i], "|") == 0)
        {
            return 1; // Pipe symbol found
        }
    }
    return 0; // Pipe symbol not found
}

/*----------------------------IO Redirection-------------------------------*/
int contains_IO(tokenlist *tokens)
{
    for (int i = 0; i < tokens->size; i++)
    {
        if (strcmp(tokens->items[i], ">") == 0 || strcmp(tokens->items[i], "<") == 0)
        {
            return 1; // IO symbol found
        }
    }
    return 0; // IO symbol not found
}

/*---------------------------------------------------------------------------*/

void execute_command(tokenlist *tokens);

int main()
{
    initializeBackgroundProcessing();

    // Last three valid commands
    char *lastValidCommands[3] = {NULL, NULL, NULL};

    int status = 1;
    while (status == 1)
    {
        char *userInput = prompt_f(); // Get user input from the prompt

        // Tokenize the input
        tokenlist *tokens = get_tokens(userInput);

        expand_environment_variables(tokens);
        expand_tilde(tokens);

        int isBackground = 0;
        if (tokens->size > 0 && strcmp(tokens->items[tokens->size - 1], "&") == 0)
        {
            isBackground = 1;
            tokens->items[tokens->size - 1] = NULL;
            tokens->size--;
        }

        if (strcmp(tokens->items[0], "jobs") == 0)
        {
            listBackgroundJobs();
        }
        else if (contains_pipe(tokens))
        {
            pipe_commands(tokens);
        }
        else if (contains_IO(tokens))
        {
            IO_compare(tokens); // Check for IO redirection
        }
        else
        {
            if (isBackground)
            {
                pid_t pid = fork();
                if (pid == 0)
                {
                    execute_command(tokens);
                    exit(EXIT_SUCCESS);
                }
                else if (pid > 0)
                {
                    addBackgroundJob(pid, userInput);
                }
                else
                {
                    perror("fork");
                }
            }
            else if (strcmp(tokens->items[0], "cd") == 0 || strcmp(tokens->items[0], "exit") == 0)
            {
                status = execute_builtin(tokens);
            }
            else
            {
                execute_command(tokens);
            }
        }

        // Update the last three valid commands
        for (int i = 2; i > 0; i--)
        {
            lastValidCommands[i] = lastValidCommands[i - 1];
        }
        lastValidCommands[0] = strdup(userInput);

        updateBackgroundJobs();

        // printf("after status func call");
        free_tokens(tokens); // Free the tokens after processing
        free(userInput);     // Free the user input
    }

    // Print the last three valid commands
    for (int i = 0; i < 3; i++)
    {
        if (lastValidCommands[0] == NULL)
        {
            printf("No commands in history\n");
        }
        else if (lastValidCommands[i] != NULL)
        {
            printf("Command %d: %s\n", i + 1, lastValidCommands[i]);
        }

        free(lastValidCommands[i]);
    }

    return 0;
}
