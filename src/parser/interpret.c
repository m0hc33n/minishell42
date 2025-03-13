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
	if (step == 0 && (status = minishell_separate(token)))
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
	bool		*asterisk;
	t_status	status;

	asterisk = (bool *)malloc(sizeof(bool) * minishell_strlen(token->tvalue));
	if (!asterisk)
		return (STATUS_MALLOCERR);
	decide_asterisk(token->tvalue, asterisk);
	if ((status = minishell_asterisk(token, asterisk)))
		return (free(asterisk), status);
	return (free(asterisk), STATUS_SUCCESS);
}

static void		decide_asterisk(char *value, bool *asterisk)
{
	uint32_t	i;
	bool		flag[2];

	i = 0;
	flag[0] = false;
	flag[1] = false;
	while (value[i])
	{
		if (*value == CHAR_DOUBLE_QUOTE && !flag[0])
			flag[1] = !flag[1];
		else if (*value == CHAR_SINGLE_QUOTE && !flag[1])
			flag[0] = !flag[0];
		asterisk[i] = (!flag[0] && !flag[1]);
		i += 1;
	}
}
