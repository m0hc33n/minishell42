#include "../../inc/parser.h"

static char	*extract_key(char *content, uint32_t *end);

char	*minishell_expand(char *content, t_env *env) //9ssem a jemmi hhh
{
	char		*result = 0;
	char		*save = 0;
	char		*temp = 0;
	char		*key = 0;
	char		*value = 0;
	char		*group = 0;
	uint32_t	s = 0;
	uint32_t	e = 0;
	
	while (content[s])
	{
		while (content[e] && content[e] != CHAR_DOLLAR_SIGN)
			e += 1;
		content[e] = 0;
		temp = minishell_strdup(content + s);
		content[e] = CHAR_DOLLAR_SIGN;
		if (!temp)
			return (NULL);
		key = extract_key(content, &e);
		if (!key)
			return (free(temp), NULL);
		value = minishell_getvalue(env, key);
		free(key);
		if (!value)
			return (free(temp), NULL);
		if (!result)
			result = minishell_strjoin(temp, value); // check return
		else
		{
			save = result;
			group = minishell_strjoin(temp, value);
			result = minishell_strjoin(result, group);
			free(save);
			free(group);
		}
		free(temp);
		free(value);
		s = e;
	}
	return (result);
}

static char	*extract_key(char *content, uint32_t *end)
{
	char		*key;
	uint32_t	start;

	start = *end;
	while (content[*end] && !minishell_isspace(content[*end]))
		*end += 1;
	key = (char *)malloc(sizeof(char) * (*end - start + 1));
	if (!key)
		return (NULL);
	minishell_strlcpy(key, content + start, *end - start + 1);
	return (key);
}
