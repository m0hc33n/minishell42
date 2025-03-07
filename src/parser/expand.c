#include "../../inc/parser.h"

static char	*extract_key(char *content, uint32_t *end);
static bool	is_separator(char c);

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
	char		switcher;
	
	if (!minishell_strchr(content, CHAR_DOLLAR_SIGN))
		return (minishell_strdup(content));
	while (s < minishell_strlen(content))
	{
		while (content[e] && !is_separator(content[e]))
			e += 1;
		switcher = content[e];
		content[e] = 0;
		temp = minishell_strdup(content + s);
		content[e] = switcher;
		if (!temp)
			return (NULL);
		key = extract_key(temp, &e);
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
	while (content[*end] && !is_separator(content[*end])) //modify separators
		*end += 1;
	key = (char *)malloc(sizeof(char) * (*end - start + 1));
	if (!key)
		return (NULL);
	minishell_strlcpy(key, content + start, *end - start + 1);
	if (*end == start)
		*end += 1;
	return (key);
}

static bool	is_separator(char c)
{
	if (c == 32 || (9 <= c && c <= 13))
		return (true);
	if (c == CHAR_DOLLAR_SIGN)
		return (true);
	return (false);
}