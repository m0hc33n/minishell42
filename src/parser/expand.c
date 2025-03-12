#include "../../inc/parser.h"

static bool	is_separator(char c);

typedef struct s_result
{
	char	*result;
	char	*saver;
	char	*key;
	char	*value;
	char	*prefix;
	char	*suffix;
	bool	flag[2];
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
		if (buff.result[s] == CHAR_DOLLAR_SIGN && !buff.flag[0])
		{
			e = s + 1;
			while (buff.result[e] && !is_separator(buff.result[e]))
				e += 1;
			buff.key = (char *)malloc(sizeof(char) * (e - s + 1));
			minishell_strlcpy(buff.key, buff.result + s, e - s + 1);
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
		{
			if (buff.result[s] == CHAR_SINGLE_QUOTE && !buff.flag[1])
				buff.flag[0] = !buff.flag[0];
			else if (buff.result[s] == CHAR_DOUBLE_QUOTE && !buff.flag[0])
				buff.flag[1] = !buff.flag[1];
			s += 1;
		}
	}
	return (buff.result);
}

static bool	is_separator(char c) // !!!!! need all cases a jemmi
{
	if (minishell_isspace(c))
		return (true);
	if (c == CHAR_ASTERISK)
		return (true);
	if (c == CHAR_SINGLE_QUOTE)
		return (true);
	if (c == CHAR_DOUBLE_QUOTE)
		return (true);
	if (c == CHAR_DOLLAR_SIGN)
		return (true);
	return (false);
}
