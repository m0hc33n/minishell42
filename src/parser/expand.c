#include "../../inc/parser.h"

static char	*extract_key(char *content, uint32_t *end);

static bool	is_separator(char c);

typedef struct s_result
{
	char	*result;
	char	*saver;
	char	*key;
	char	*value;
	char	*prefix;
	char	*suffix;
}	t_result;

char	*minishell_expand(char *content, t_env *env) //9ssem a jemmi hhh
{
	t_result	buff;
	uint32_t	s;
	uint32_t	e;
	
	minishell_memset(&buff, 0, sizeof(t_result));
	s = 0;
	e = 0;
	if (!minishell_strchr(content, CHAR_DOLLAR_SIGN))
		return (minishell_strdup(content));
	buff.result = minishell_strdup(content); // check fail
	while (buff.result[s]) // gad freeer l struct mf
	{
		if (buff.result[s] == CHAR_DOLLAR_SIGN)
		{
			e = s + 1;
			while (buff.result[e] && !is_separator(content[e]))
				e += 1;
			buff.key = (char *)malloc(sizeof(char) * (e - s + 1));
			buff.value = minishell_getvalue(env, buff.key);
			free(buff.key);
			buff.result[s] = 0;
			buff.prefix = minishell_strdup(buff.result);
			buff.result[s] = CHAR_DOLLAR_SIGN;
			buff.suffix = minishell_strdup(buff.result + e);
			buff.result = minishell_strjoin(buff.prefix, buff.value);
			buff.result = minishell_strjoin(buff.result, buff.suffix);
			s = e;
		}
		else
			s += 1;
	}
	return (buff.result);
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