#ifndef IO_REDIRECT_H
#define IO_REDIRECT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "path_search.h"
#include "lexer.h"

void IO_redirect(char **args, char *input_file, char *output_file);
void IO_compare(tokenlist *tokens);

#endif // IO_REDIRECT_H
