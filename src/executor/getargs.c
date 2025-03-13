#include "../../inc/executor.h"

static uint32_t getargs_argc(t_root *root)
{	
	uint32_t	sz;

	sz = 0;
	if (root)
	{
		while (root)
		{
			sz++;
			root = root->right;
		}
		return (sz);
	}
	return (0);
}	

static char **getargs_init(t_root *root, uint32_t *argc)
{
	char		**argv;

	if (root)
	{
		*argc = getargs_argc(root);
		argv = (char **)minishell_calloc(1, sizeof(char *) * (*argc + 1));
		if (!argv)
			return (NULL);
		return (argv);
	}
	return (NULL);
}

char	**executor_getargs(t_root *root, t_env *env, int32_t exit_code)
{
	char		**argv;
	uint32_t	argc;
	uint32_t	count;

	count = 0;
	if (!minishell_translate(root, env, minishell_i32tostr(exit_code)))
		return (NULL);
	argv = getargs_init(root, &argc);
	if (!argv || !argc)
		return (NULL);
	if (root->tvalue == TTOKEN_COMMAND)
	{
		argv[count++] = root->tvalue;
		root = root->right;
	}
	while (root)
	{
		argv[count++] = root->tvalue;
		root = root->right;
	}
	argv[count] = 0;
	return (argv);
}
