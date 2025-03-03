#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

// do i need to free agrv and *argv[i] each time ?? answer: yes
// btw i always check if l_env is null, it can be if u unset everything
//  have to modify l_env in unset so need to pass t_env **l_env !!

// !!!TODO!!! free argv inside builtins


t_status	minishell_echo(char **argv, t_env *l_env);
t_status	minishell_cd(char **argv, t_env *l_env);
t_status	minishell_pwd(char **argv, t_env *l_env);
t_status	minishell_export(char **argv, t_env *l_env);
t_status	minishell_unset(char **argv, t_env **l_env);
t_status	minishell_env(char **argv, t_env *l_env);
t_status	minishell_exit(t_minishell *minishell);


// m0hc33n

# define EXPORT	"export"
# define UNSET	"unset"
# define ECHO	"echo"
# define EXIT	"exit"
# define ENV	"env"
# define PWD	"pwd"
# define CD		"cd"

bool		minishell_isbuiltin(char *cmd);
void		exec_builtin(t_minishell *minishell, char **argv);

#endif