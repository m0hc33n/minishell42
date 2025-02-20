#include "../../inc/executor.h"

void	exec_cmd(t_root *root, int32_t *exit_code)
{
	char		**argv;
	uint32_t	procid;

	argv = executor_getargs(root); //TODO
	// TODO : ADD SUPPORT TO `*`
	procid = fork();
	if (procid == CHILD_PROCESS)
		execve(argv[0], argv, NULL);
	wait(exit_code);
	free(argv);
}
