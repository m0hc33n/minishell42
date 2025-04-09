#include "../../inc/parser.h"

static bool		*get_flags_quotes(char *s);

t_status	minishell_remove(t_token *token)
{
	t_quotes	vars;

	minishell_memset(&vars, 0, sizeof(t_quotes));
	vars.flags = get_flags_quotes(token->tvalue);
	if (!vars.flags)
		return (STATUS_MALLOCERR);
	while (token->tvalue[vars.i[0]])
		vars.fsize += (vars.flags[vars.i[0]++] == true);
	if (vars.fsize < minishell_strlen(token->tvalue))
	{
		vars.fvalue = (char *)malloc(sizeof(char) * (vars.fsize + 1));
		if (!vars.fvalue)
			return (minishell_free((void **)&vars.flags), STATUS_MALLOCERR);
		while (token->tvalue[vars.i[1]])
		{
			if (vars.flags[vars.i[1]++])
				vars.fvalue[vars.i[2]++] = token->tvalue[vars.i[1] - 1];
		}
		vars.fvalue[vars.i[2]] = 0;
		minishell_free((void **)&token->tvalue);
		token->tvalue = vars.fvalue;
	}
	return (minishell_free((void **)&vars.flags), STATUS_SUCCESS);
}

static bool	*get_flags_quotes(char *s)
{
	bool		*qflags;
	uint32_t	len;
	bool		flag[2];
	uint32_t	i;

	len = minishell_strlen(s);
	qflags = (bool *)malloc(sizeof(bool) * len);
	if (!qflags)
		return (NULL);
	minishell_memset(flag, 0, sizeof(bool) * 2);
	i = 0;
	while (i < len)
	{
		qflags[i] = ((s[i] != CHAR_DOUBLE_QUOTE)
				&& (s[i] != CHAR_SINGLE_QUOTE));
		qflags[i] = (qflags[i] || (s[i] == CHAR_DOUBLE_QUOTE && flag[0]));
		qflags[i] = (qflags[i] || (s[i] == CHAR_SINGLE_QUOTE && flag[1]));
		if (s[i] == CHAR_DOUBLE_QUOTE && !flag[0])
			flag[1] = !flag[1];
		else if (s[i] == CHAR_SINGLE_QUOTE && !flag[1])
			flag[0] = !flag[0];
		i += 1;
	}
	return (qflags);
}
