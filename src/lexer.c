#ifndef LEXER_C
#define LEXER_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

tokenlist *new_tokenlist()
{
	tokenlist *tl = malloc(sizeof(tokenlist));
	tl->size = 0;
	tl->items = malloc(0); // Initially zero, will be resized as tokens are added
	return tl;
}

void add_token(tokenlist *tl, char *item)
{
	tl->size++;
	tl->items = realloc(tl->items, sizeof(char *) * tl->size); // Resize the array of tokens
	tl->items[tl->size - 1] = strdup(item);					   // Add the new token
}

void free_tokens(tokenlist *tl)
{
	for (int i = 0; i < tl->size; i++)
	{
		free(tl->items[i]);
	}
	free(tl->items);
	free(tl);
}

tokenlist *get_tokens(char *input)
{
	const char *delimiter = " ";
	tokenlist *tokens = new_tokenlist();

	char *token = strtok(input, delimiter);
	while (token != NULL)
	{
		add_token(tokens, token);
		token = strtok(NULL, delimiter);
	}

	return tokens;
}

tokenlist *get_path_tokens(char *input)
{
	const char *delimiter = ":";
	tokenlist *tokens = new_tokenlist();

	char *token = strtok(input, delimiter);
	while (token != NULL)
	{
		add_token(tokens, token);
		token = strtok(NULL, delimiter);
	}

	return tokens;
}

#endif
