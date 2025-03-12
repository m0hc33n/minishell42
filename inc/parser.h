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
t_status	minishell_translate(t_token *root, t_env *env);

/* interpret.c */
t_status	minishell_interpret(t_token *token, t_env *env, bool flag, uint8_t step);

/* expand.c */
char		*minishell_expand(char *content, t_env *env); //norm + fail safe

/* separate.c */
t_status	minishell_separate(t_token *token);

/* asterisk.c */
t_status	minishell_asterisk(t_token *token, bool *asterisk); // TODO

/* pattern.c */
t_fixe		*split_pattern(char *pattern, bool *asterisk);
bool		matches_pattern(t_fixe *fixe, char *s);

/* splitter.c */
char		**asterisk_split(char *pattern, bool *asterisk);

#endif


/*
TRANSLATION STEPS:

	- EXPAND ENV VARIABLES
	- EXPAND ASTERISK
	- SPLIT TOKEN INTO TOKENS
	- NULLIFY INITIAL TOKEN (TO BE REMOVED DURING CLEANING)
*/