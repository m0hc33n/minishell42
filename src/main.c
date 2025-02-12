#include "minishell.h"


/*
	TODO
		* s_minisell init
		* segnal init
*/
t_status	minishell_init(t_minishell **minishell)	
{
	if (minishell)
	{
		*minishell = (t_minishell *)malloc(sizeof(t_minishell));
		if (!minishell)
			return (STATUS_MALLOCERR);
		(*minishell)->prompt = PROMPT;
		return (STATUS_SUCCESS);
	}
	return (STATUS_MSINITERROR);
}

int main(int ac, char **av, char **env)
{
	t_minishell	*minishell;
	t_status	status;

	if (! (status = minishell_init(&minishell))) // TODO > minishell_init
		return (minishell_error(status, NULL)); // TODO > minishell_error
	while (true)
	{
		minishell->cmdline = readline(minishell->prompt);
		if (!(status = minishell_lexer(minishell)))	 // WORKING ON IT
			return (minishell_error(status, &minishell));
		if (!(status = minishell_parser(minishell)))  // TODO > minishell_parser
			return (minishell_error(status, &minishell));
		if (!(status = minishell_executer(minishell))) // TODO > minishell_executor
			return (minishell_error(status, &minishell));
		minishell_reset(&minishell); // TODO > minishell_reset
	}
}
