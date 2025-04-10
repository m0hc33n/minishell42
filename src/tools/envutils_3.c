#include "../../inc/tools.h"

static uint32_t	get_env_size(t_env *env);
static bool		fill_envp(t_env *env, char **envp);

char	**minishell_getenvp(t_env *env)
{
	uint32_t	size;
	char		**envp;

	if (!env)
		return (NULL);
	size = get_env_size(env);
	envp = (char **)malloc(sizeof(char *) * (size + 1));
	if (!envp)
		return (NULL);
	minishell_memset(envp, 0, sizeof(char *) * (size + 1));
	if (fill_envp(env, envp))
		return (minishell_free_arr(envp), NULL);
	return (envp);
}

static uint32_t	get_env_size(t_env *env)
{
	uint32_t	size;
	t_env		*node;

	size = 0;
	node = env;
	while (node)
	{
		size += 1;
		node = node->next_key;
	}
	return (size);
}

static bool	fill_envp(t_env *env, char **envp)
{
	uint32_t	i;
	t_env		*node_i;
	uint32_t	kl;
	uint32_t	vl;

	i = 0;
	node_i = env;
	while (node_i)
	{
		kl = minishell_strlen(node_i->key);
		vl = minishell_strlen(node_i->value);
		printf("%s=%s\n", node_i->key, node_i->value);
		printf("[size is %u]\n", kl + vl + 2);
		envp[i] = (char *)malloc(sizeof(char) * (kl+ vl + 2));
		if (!envp[i])
			return (false);
		minishell_strlcpy(envp[i], node_i->key, kl + 1);
		minishell_strlcpy(envp[i] + kl, "=", 2);
		minishell_strlcat(envp[i] + kl + 2, node_i->value, kl + vl + 2);
		i += 1;
		node_i = node_i->next_key;
	}
	return (true);
}
