#ifndef PIPING_H
#define PIPING_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "path_search.h"
#include "lexer.h"

void pipe_commands(tokenlist *tokens);

#endif