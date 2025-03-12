#include "../../inc/parser.h"

/*-------------------------------------------------------------------------------------*/
static t_status	interpret_dollar(t_token *token, t_env *env);
static t_status	interpret_asterisk(t_token *token);

t_status	minishell_interpret(t_token *token, t_env *env, bool flag, uint8_t step)
{
	t_status	status;

	if (minishell_strchr(token->tvalue, '$') && step == 0)
	{
		if ((status = interpret_dollar(token, env)))
			return (status);
	}
	if (minishell_strchr(token->tvalue, '*') && step == 1 && flag)
	{
		if ((status = interpret_asterisk(token)))
			return (status);
	}
	if ((status = minishell_separate(token)))
		return (status);
	return (STATUS_SUCCESS);
}
/*-------------------------------------------------------------------------------------*/
static t_status	interpret_dollar(t_token *token, t_env *env)
{
	char	*e_value;

	e_value = minishell_expand(token->tvalue, env);
	if (!e_value)
		return (STATUS_MALLOCERR);
	free(token->tvalue);
	token->tvalue = e_value;
	return (STATUS_SUCCESS);
}
/*-------------------------------------------------------------------------------------*/
static void		decide_asterisk(char *value, bool *asterisk);

static t_status	interpret_asterisk(t_token *token)
{
	uint32_t	a_count;
	char		*value;
	bool		*asterisk;
	t_status	status;

	a_count = 0;
	value = token->tvalue;
	while (*value)
	{
		if (*value == '*')
			a_count += 1;
		value += 1;
	}
	asterisk = (bool *)malloc(sizeof(bool) * a_count);
	if (!asterisk)
		return (STATUS_MALLOCERR);
	decide_asterisk(token->tvalue, asterisk);
	if ((status = minishell_asterisk(token, asterisk)))
		return (free(asterisk), status);
	return (free(asterisk), STATUS_SUCCESS);
}

static void		decide_asterisk(char *value, bool *asterisk)
{
	bool		flag[2];
	uint32_t	count;

	flag[0] = false;
	flag[1] = false;
	count = 0;
	while (*value)
	{
		if (*value == CHAR_DOUBLE_QUOTE && !flag[0])
			flag[1] = !flag[1];
		else if (*value == CHAR_SINGLE_QUOTE && !flag[1])
			flag[0] = !flag[0];
		else if (*value == '*')
		{
			asterisk[count] = (!flag[0] && !flag[1]);
			count += 1;
		}
		value += 1;
	}
}
