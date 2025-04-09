#include "../../inc/executor.h"

static void	exec_failed(t_root *cmd_node, int32_t status)
{
	if (status == STATUS_CMDNOTFOUND)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, cmd_node->tvalue,
			minishell_strlen(cmd_node->tvalue));
		write(STDERR_FILENO, ": command not found\n", 20);
	}
}

static void	exec_exec(t_minishell *minishell, char **argv)
{
	int32_t	status;
	pid_t	pid;

	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		execve(argv[0], argv, NULL);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		minishell->exit_code = 0;
	else
		minishell->exit_code = WEXITSTATUS(status);
}

void	exec_cmd(t_minishell *minishell, t_root *cmd_node)
{
	char	**argv;
	pid_t	pid;
	int32_t	status;

	status = 0;
	argv = executor_getargs(cmd_node, minishell, (t_status *)&status);
	if (!argv)
	{
		exec_failed(cmd_node, status);
		return ;
	}
	if (minishell_isbuiltin(argv[0]))
		minishell->exit_code = exec_builtin(minishell, argv);
	else
		exec_exec(minishell, argv);
	minishell_free((void **)&argv);
}
