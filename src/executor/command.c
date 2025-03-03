#include "../../inc/executor.h"

void	exec_cmd(t_minishell *minishell, t_root *root, int32_t output_fd)
{
	char		**argv;
	pid_t		pid;
	int32_t		status;

	argv = executor_getargs(root);
	if (minishell_isbuiltin(argv[0]))
			minishell->exit_code = exec_builtin(minishell, argv);
	else
	{
		pid = fork();
		if (pid == CHILD_PROCESS)
		{
			if (output_fd != 1)
				dup2(output_fd, STDOUT_FILENO);
			printf("[DEBUG] : argv[0] : %s\n", argv[0]);
			execve(argv[0], argv, NULL);
			exit(EXIT_FAILURE);
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
			minishell->exit_code = 0;
		else
			minishell->exit_code = WEXITSTATUS(status);
	}
	minishell_free_arr(argv);
}
