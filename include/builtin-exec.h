// Internal Execution

#ifndef BUILDIN_EXEC_H
#define BUILDIN_EXEC_H

#include "path_search.h"
#include "env_expansion.h"
#include "backgroundPro.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int execute_builtin(tokenlist *tokens);
void change_directory(tokenlist *tokens);

#endif
