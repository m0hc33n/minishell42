#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

t_status	minishell_parser(t_minishell *minishell);

t_status	minishell_translate(t_token *root, t_env *env, char *str_exitcode);

t_status	minishell_interpret(t_token *token, t_env *env, t_args args);
bool		setbool(bool *flag, bool value);

char		*minishell_expand(char *content, t_env *env, t_args args);

t_status	minishell_separate(t_token *token);

t_status	minishell_asterisk(t_token *token, bool *asterisk);

t_fixe		*minishell_analyse(char *pattern, bool *asterisk);

bool		minishell_matcher(t_fixe *fixe, char *s);

t_status	minishell_remove(t_token *token);

#endif