#include "../../inc/tools.h"

void	minishell_setstatus(t_minishell *minishell, int status)
{
	if (WIFEXITED(status))
		minishell->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		minishell->exit_code = 128 + WTERMSIG(status);
	else
		minishell->exit_code = 1;
}
