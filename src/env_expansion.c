
// env_expansion.c

#include <stdlib.h> // For getenv() and free()
#include <string.h> // For strncmp() and strdup()
#include "env_expansion.h"

void expand_environment_variables(tokenlist *tokens)
{
    for (int i = 0; i < tokens->size; i++)
    {
        char *token = tokens->items[i];

        if (token[0] == '$')
        {
            char *env_var_name = token + 1; // Skip the '$'
            char *value = getenv(env_var_name);

            if (value != NULL)
            {
                free(tokens->items[i]);           // Free the old token memory
                tokens->items[i] = strdup(value); // Replace with new value
            }
            else
            {
                // Handle undefined environment variables as needed
                // Example: replace undefined variables with an empty string
                free(tokens->items[i]);
                tokens->items[i] = strdup("");
            }
        }
    }
}

void expand_tilde(tokenlist *tokens)
{
    char *home = getenv("HOME"); // Get the value of the HOME environment variable

    for (int i = 0; i < tokens->size; i++)
    {
        char *token = tokens->items[i];

        // Check if the token is a standalone tilde or starts with "~/"
        if (strcmp(token, "~") == 0)
        {
            // Replace the standalone tilde with the value of HOME
            free(tokens->items[i]);
            tokens->items[i] = strdup(home);
        }
        else if (strncmp(token, "~/", 2) == 0)
        {
            // Replace "~/path" with "$HOME/path"
            char *rest_of_path = token + 1; // Use +1 to include the "/" in rest_of_path

            // Allocate enough space for the expanded path (+1 for the null terminator)

            char *expanded_path = (char *)malloc(strlen(home) + strlen(rest_of_path) + 1);
            strcpy(expanded_path, home);
            strcat(expanded_path, rest_of_path);

            free(tokens->items[i]);
            tokens->items[i] = expanded_path;
        }
    }
}
