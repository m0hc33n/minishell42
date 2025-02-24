#include "../../inc/executor.h"

void	exec_cmd(t_root *root, int32_t *exit_code)
{
	char		**argv;
	uint32_t	pid;
	uint32_t	status;

	// TODO : ADD SUPPORT TO `*`
	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		argv = executor_getargs(root);
		execve(argv[0], argv, NULL);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		*exit_code = 0;
	else
		*exit_code = WEXITSTATUS(status);
}
