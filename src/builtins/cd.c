#include "../../inc/builtins.h"

static t_status	cd(char *dest, t_env *l_env);
static t_status	update_wd(char *old_pwd, char *new_pwd, t_env *l_env);

t_status	minishell_cd(char **argv, t_env *l_env)
{
	char	*dest;

	if (argv && l_env)
	{
		if (argv[1] && argv[2])
		{
			write(STDERR_FILENO, "minishell_cd: too many arguments\n", 33);
			return (STATUS_FAILURE);
		}
		if (!argv[1])
			dest = minishell_getvalue(l_env, "$HOME");
		else
			dest = minishell_strdup(argv[1]);
		if (!dest)
				return (STATUS_MALLOCERR);
		return (cd(dest, l_env));
	}
	return (STATUS_FAILURE);
}

static t_status	cd(char *dest, t_env *l_env)
{
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = pwd(l_env);
	if (!old_pwd)
		return (free(dest), STATUS_FAILURE);
	if (chdir(dest))
		return (free(old_pwd), free(dest), STATUS_FAILURE);
	free(dest);
	new_pwd = pwd(l_env);
	if (!new_pwd) // should i cd back to oldpwd or just stay in dest and say it failed hmm
		return (free(old_pwd), STATUS_FAILURE);
	if (update_wd(old_pwd, new_pwd, l_env))
		return (STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}

static t_status	update_wd(char *old_pwd, char *new_pwd, t_env *l_env)
{
	char	*save;

	save = old_pwd;
	old_pwd = minishell_strjoin("OLDPWD=", old_pwd);
	free(save);
	if (!old_pwd)
		return (free(new_pwd), STATUS_MALLOCERR);
	save = new_pwd;
	new_pwd = minishell_strjoin("PWD=", new_pwd);
	free(save);
	if (!new_pwd)
		return (free(old_pwd), STATUS_MALLOCERR);
	if (export(old_pwd, l_env) || export(new_pwd, l_env))
		return (free(old_pwd), free(new_pwd), STATUS_MALLOCERR);
	return (free(old_pwd), free(new_pwd), STATUS_SUCCESS);
}
