#include "../../inc/executor.h"

static char **getargs_init(t_root *root, int32_t *argc)
{
	char		**argv;

	if (root)
	{
		*argc = getargs_argc(root);
		argv = (char **)malloc(sizeof(char *) * *argc);
		if (argv)
			return (NULL);
		return (argv);
	}
	return (NULL);
}

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

char	**executor_getargs(t_root *root)
{
	char		**argv;
	uint32_t	argc;
	uint32_t	count;

	count = 0;
	argv = NULL;
	argv = getargs_init(root, &argc);
	if (!argv || !argc)
		return (NULL);
	if (root->value == TTOKEN_COMMAND)
	{
		argv[count++] = root->value;
		root = root->right;
	}
	while (root)
	{
		argv[count++] = root->value;
		root = root->right;
	}
	return (argv);
}
