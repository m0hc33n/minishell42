#include "../../inc/builtins.h"

t_status	minishell_pwd(char **argv, t_env *l_env) // fix and check bash behavior biman kolani
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

char		*pwd(t_env *l_env)
{
	char	*cwd;

	(void)l_env;
	cwd = getcwd(NULL, 0);
	return (cwd);
}