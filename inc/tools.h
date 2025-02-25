#ifndef TOOLS_H
# define TOOLS_H

#include "minishell.h"

// STRING FUNCTIONS
bool		minishell_strcmp(char *s1, char *s2);
bool		minishell_strchr(const char *s, int c);
uint32_t	minishell_strlen(const char *s);
uint64_t	minishell_strlcpy(char *dst, const char *src,
				uint64_t dstsize);

// MEMORY UTILS
void		*minishell_memset(void *b, int c, size_t len);
void		*minishell_calloc(size_t count, size_t size);

// IS FUNCTIONS
bool		minishell_isspace(int c);
bool		minishell_iscmdsep(int c);

// ERROR HANDLING
void	minishell_error(t_status status);

// CLEAN
void    minishell_reset(t_minishell **minishell);
void    minishell_free_arr(char **arr);

#endif