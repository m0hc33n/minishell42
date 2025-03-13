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

typedef struct s_chunk
{
	char			*content;
	struct s_chunk	*next;
}	t_chunk;

/* parse.c */
t_status	minishell_parser(t_minishell *minishell);

/* translate.c */
t_status	minishell_translate(t_token *root, t_env *env, char *str_exitcode);

/* interpret.c */
t_status	minishell_interpret(t_token *token, t_env *env, bool flag, uint8_t step);

/* expand.c */
char		*minishell_expand(char *content, t_env *env); //norm + fail safe

/* separate.c */
t_status	minishell_separate(t_token *token);

/* asterisk.c */
t_status	minishell_asterisk(t_token *token, bool *asterisk);

/* analyse.c */
t_fixe		*minishell_analyse(char *pattern, bool *asterisk);

/* match.c */
bool		minishell_matcher(t_fixe *fixe, char *s);

#endif