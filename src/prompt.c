#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "prompt.h"

#define HOST_NAME_MAX 255
#define PATH_MAX 4096

char cwd[PATH_MAX]; // store the current working directory.

char *prompt_f()
{
    char hostname[HOST_NAME_MAX];    // array to store the host name.
    char cwd[PATH_MAX];              // array to store the current working directory path.
    char *username = getenv("USER"); // Retrieves the username from the environment variables and stores it in username.
                                     // Calls gethostname to get the name of the current host. If it returns a non-zero value (indicating an error), the if statement is executed
    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        perror("gethostname");
        exit(EXIT_FAILURE);
    }
    // Gets the current working directory. If getcwd fails (returns NULL), the if statement is executed
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }
    // arge enough to hold the path, host name, and additional characters for the command prompt format
    char prompt[PATH_MAX + HOST_NAME_MAX + 50];
    // formats and stores a string into the prompt array
    snprintf(prompt, sizeof(prompt), "%s@%s:%s> ", username, hostname, cwd);

    printf("%s", prompt);
    fflush(stdout);

    // dynamically allocates memory for a character array to store user input.
    char *userInput = (char *)malloc(1024 * sizeof(char));
    // reads input and store them in userInput
    if (fgets(userInput, 1024, stdin) == NULL)
    {
        perror("fgets");
        exit(EXIT_FAILURE);
    }

    size_t len = strlen(userInput);
    if (len > 0 && userInput[len - 1] == '\n')
    {
        userInput[len - 1] = '\0';
    }

    return userInput;
}
