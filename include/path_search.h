// path_search.h

#ifndef PATH_SEARCH_H
#define PATH_SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "lexer.h"

char *find_command_in_path(const char *command);
void execute_command(tokenlist *tokens);

#endif // PATH_SEARCH_H
