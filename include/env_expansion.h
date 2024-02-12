// env_expansion.h
#ifndef ENV_EXPANSION_H
#define ENV_EXPANSION_H

#include "lexer.h"

void expand_environment_variables(tokenlist *tokens);
void expand_tilde(tokenlist *tokens);

#endif // ENV_EXPANSION_H
