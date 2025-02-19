#include "minishell.h"

/*
	TODO
		* s_minisell init
		* segnal init
*/
t_status	minishell_init(t_minishell **minishell, char **env)	
{
	if (minishell)
	{
		*minishell = (t_minishell *)malloc(sizeof(t_minishell));
		if (!minishell)
			return (STATUS_MALLOCERR);
		(*minishell)->prompt = PROMPT;
		(*minishell)->env = minishell_get_env(env); // TODO
		if (!(*minishell)->env)
			return (STATUS_ENVFAILED);
		return (STATUS_SUCCESS);
	}
	return (STATUS_MSINITERROR);
}

static t_status minishell(t_minishell *minishell)
{
	t_status	status;

	minishell->cmdline = readline(minishell->prompt);
	status = minishell_lexer(minishell);
	if (status)
		return (status);
	status = minishell_parser(minishell);
	if (status)
		return (status);
	status = minishell_executer(minishell); //undere dev by `m0hc33n` (estimate time: 7days)
	if (status)
		return (status);
	return (STATUS_SUCCESS);
} 

int main(int ac, char **av, char **env)
{
	t_minishell	*ms;
	t_status	status;

	if (! (status = minishell_init(&ms, env))) // TODO > minishell_init
		return (minishell_error(status, NULL)); // TODO > minishell_error
	while (true)
	{
		status = minishell(ms);
		if (status)
			minishell_error(status); // TODO > minishell_error
		minishell_reset(&ms); // TODO > minishell_reset
	}
}
