// C file for builtin-exec.h

#include "builtin-exec.h" // Main Header File

int execute_builtin(tokenlist *tokens)
{
    char **args = tokens->items;

    if (strcmp(args[0], "exit") == 0)
    {
        return 0;
    }
    else if (strcmp(args[0], "cd") == 0)
    {
        change_directory(tokens);
        return 1;
    }
    return 1;
}

void change_directory(tokenlist *tokens)
{
    char **args = tokens->items;

    char *path = tokens->size > 1 ? tokens->items[1] : getenv("HOME");

    if (tokens->size > 2)
    {
        fprintf(stderr, "cd: too many arguments\n");
        return;
    }
    else if (access(args[1], F_OK) != 0)
    { // If the target does not exist, print error message
        fprintf(stderr, "cd: %s: Target does not exist\n", args[1]);
    }
    else if (access(args[1], R_OK) != 0)
    { // If the target is not a directory, print error message
        fprintf(stderr, "cd: %s: Target not a directory\n", args[1]);
    }
    else
    {
        // Change the current working directory to the target
        chdir(path);
    }
}