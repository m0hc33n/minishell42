#ifndef TOOLS_H
# define TOOLS_H

#include "minishell.h"

// STRING FUNCTIONS
bool		minishell_strchr(const char *s, int c);
uint32_t	minishell_strlen(const char *s);

// IS FUNCTIONS
bool		minishell_isspace(int c);
bool		minishell_iscmdsep(int c);

#endif