#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "piping.h"

void pipe_commands(tokenlist *tokens)
{
    int num_pipes = 0;
    for (int i = 0; i < tokens->size; i++)
    {
        if (strcmp(tokens->items[i], "|") == 0)
        {
            num_pipes++;
        }
    }

    int cmd_start = 0;
    int cmd_end = 0;
    int pipe_fds[2];

    for (int i = 0; i <= num_pipes; i++)
    {
        cmd_end = cmd_start;
        while (cmd_end < tokens->size && strcmp(tokens->items[cmd_end], "|") != 0)
        {
            cmd_end++;
        }

        if (i < num_pipes)
        {
            if (pipe(pipe_fds) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        { // Child process
            if (i > 0)
            {
                dup2(pipe_fds[0], STDIN_FILENO);
                close(pipe_fds[0]);
            }
            if (i < num_pipes)
            {
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[1]);
            }

            for (int j = cmd_start; j < cmd_end; j++)
            {
                tokens->items[j] = strdup(tokens->items[j]);
            }
            tokens->items[cmd_end] = NULL;

            execv(tokens->items[cmd_start], &tokens->items[cmd_start]);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else
        { // Parent process
            if (i > 0)
            {
                close(pipe_fds[0]);
            }
            if (i < num_pipes)
            {
                close(pipe_fds[1]);
            }
            cmd_start = cmd_end + 1;
        }
    }

    // Wait for all child processes to complete
    for (int i = 0; i <= num_pipes; i++)
    {
        wait(NULL);
    }
}
