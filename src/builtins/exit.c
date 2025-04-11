#include "../../inc/builtins.h"

t_status	minishell_exit(t_minishell *minishell)
{
	write(STDOUT_FILENO, "exit\n", 5);
	minishell_cleanup(minishell, STATUS_SUCCESS);
	return (STATUS_SUCCESS);
}
