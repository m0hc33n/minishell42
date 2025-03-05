#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

typedef enum e_default_priority
{
	PRIORITY_CRITICAL = 1,
	PRIORITY_HIGH,
	PRIORITY_MEDIUM,
	PRIORITY_IDLE,
	PRIORITY_REMOVE
}	t_default_priority;

// to be called before execution
t_status	minishell_parser(t_minishell *minishell);

// to be called during execution
t_status	minishell_translate(t_token *root, t_env *env);

// helper functions
t_status	minishell_interpet(t_token *token, t_env *env); // TODO

t_status	minishell_asterisk(t_token *token);
t_fixe		*split_pattern(char *pattern);
bool		matches_pattern(t_fixe *fixe, char *s);

char		*minishell_quotes(char *value);

#endif