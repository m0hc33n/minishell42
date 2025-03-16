#include "../../inc/builtins.h"

t_status	minishell_pwd(char **argv, t_env *l_env)
{
	char	*cwd;

	if (argv && l_env)
	{
		cwd = pwd(l_env);
		if (!cwd)
			return (STATUS_FAILURE);
		printf("%s\n", cwd);
		free(cwd);
		return (STATUS_SUCCESS);
	}
	return (STATUS_FAILURE);
}

char	*pwd(t_env *l_env) // bug: what if i delete dir and unset PWD ? 
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (minishell_getvalue(l_env, "$PWD"));
	return (cwd);
}
