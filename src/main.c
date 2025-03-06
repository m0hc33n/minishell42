#include "../inc/minishell.h"

/*
	TODO
		* s_minisell init
		* segnal init
*/
t_status	minishell_init(t_minishell **minishell, char **env)	
{
	if (minishell)
	{
		*minishell = (t_minishell *)minishell_calloc(1, sizeof(t_minishell));
		if (!minishell)
			return (STATUS_MALLOCERR);
		(*minishell)->prompt = PROMPT;
		(*minishell)->env = minishell_getenv(env);
		if (!(*minishell)->env)
			return (STATUS_ENVFAILED);
		(*minishell)->stdfd[0] = dup(STDIN_FILENO);
		(*minishell)->stdfd[1] = dup(STDOUT_FILENO);
		return (STATUS_SUCCESS);
	}
	return (STATUS_MSINITERROR);
}

static t_status minishell(t_minishell *minishell)
{
	t_status	status;


	minishell->cmdline = readline(minishell->prompt);
	if (!minishell->cmdline[0])
		return (STATUS_EMPTYCMD);
	status = minishell_lexer(minishell);
	if (status)
		return (status);
	status = minishell_parser(minishell);
	if (status)
		return (status);
	status = minishell_executor(minishell);
	if (status)
		return (status);
	return (STATUS_SUCCESS);
} 

int main(int ac, char **av, char **env)
{
	t_minishell	*ms;
	t_status	status;

	(void)av;
	(void)ac;
	status = minishell_init(&ms, env);
	if (status)
	{
		minishell_error(status);
		return (1);
	}
	while (true)
	{
		status = minishell(ms);
		if (status == STATUS_EMPTYCMD)
			continue;
		if (status)
			minishell_error(status);
		minishell_reset(&ms);
	}
}
