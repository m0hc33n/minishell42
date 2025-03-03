#include "../../inc/builtins.h"

bool		minishell_isbuiltin(char *cmd)
{
	if (minishell_strequal(cmd, EXPORT))
		return (true);
	else if (minishell_strequal(cmd, UNSET))
		return (true);
	else if (minishell_strequal(cmd, ECHO))
		return (true);
	else if (minishell_strequal(cmd, EXIT))
		return (true);
	else if (minishell_strequal(cmd, ENV))
		return (true);
	else if (minishell_strequal(cmd, PWD))
		return (true);
	else if (minishell_strequal(cmd, CD))
		return (true);
	return (false);
}

void	exec_builtin(t_minishell *minishell, char **argv)
{
	if (minishell_strequal(argv[0], EXPORT))
		minishell_export(argv, minishell->env);
	else if (minishell_strequal(argv[0], UNSET))
		minishell_unset(argv, &minishell->env);
	else if (minishell_strequal(argv[0], ECHO))
		minishell_echo(argv, minishell->env);
	else if (minishell_strequal(argv[0], ENV))
		minishell_env(argv, minishell->env);
	else if (minishell_strequal(argv[0], PWD))
		minishell_pwd(argv, minishell->env);
	else if (minishell_strequal(argv[0], CD))
		minishell_cd(argv, minishell->env);
	else if (minishell_strequal(argv[0], EXIT))
		minishell_exit(minishell);
}
