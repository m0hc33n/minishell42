#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

t_status	minishell_echo(char **argv, t_env *l_env);

t_status	minishell_cd(char **argv, t_env *l_env);

t_status	minishell_pwd(char **argv, t_env *l_env);
char		*pwd(t_env *l_env); // to be freed

t_status	minishell_export(char **argv, t_env *l_env);
t_status	export(char *arg, t_env *l_env);

t_status	minishell_unset(char **argv, t_env **l_env);

t_status	minishell_env(char **argv, t_env *l_env);

t_status	minishell_exit(t_minishell *minishell);


// m0hc33n

# define ECHO	"echo"
# define CD		"cd"
# define PWD	"pwd"
# define EXPORT	"export"
# define UNSET	"unset"
# define ENV	"env"
# define EXIT	"exit"

bool		minishell_isbuiltin(char *cmd);
t_status	exec_builtin(t_minishell *minishell, char **argv);

#endif