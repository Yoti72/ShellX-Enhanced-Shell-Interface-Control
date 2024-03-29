#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    char **items;
    size_t size;
} tokenlist;

tokenlist *get_tokens(char *input);
tokenlist *get_path_tokens(char *input);
tokenlist *new_tokenlist(void);
void add_token(tokenlist *tokens, char *item);
void free_tokens(tokenlist *tokens);