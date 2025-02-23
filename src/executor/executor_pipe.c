#include "../../inc/executor.h"

void pipeit(t_root *node, int input_fd, uint32_t *exit_code)
{
    int 	pipe_fd[2];
	char	**argv;
	pid_t	pid;

    if (node == NULL)
		return ;
    if (node->type == TTOKEN_PIPE)
        pipe(pipe_fd);

    pid = fork();
    if (pid == 0)
	{
        if (input_fd != 0)
		{
            dup2(input_fd, 0); // Redirect input
            close(input_fd);
        }
        if (node->type == TTOKEN_PIPE)
		{
            dup2(pipe_fd[1], 1); // Redirect output to pipe
            close(pipe_fd[1]);
        }
        argv = executor_getargs(node->left);
        execve(argv[0], argv, NULL);
        exit(EXIT_FAILURE);
    } 
	else if (pid > 0)
	{
        if (input_fd != 0)
            close(input_fd); // Close the old input
        if (node->type == TTOKEN_PIPE)
            close(pipe_fd[1]); // Close the write end of the pipe
        // Recursively execute the right side of the tree
		if (node->type == TTOKEN_PIPE)
       		pipeit(node->right, pipe_fd[0], exit_code);
		else
			pipeit(node->right, input_fd, exit_code);
        // Wait for the child process to finish
        waitpid(pid, &exit_code, 0);
		if (WIFEXITED(*exit_code) && WEXITSTATUS(*exit_code) != 0)
			return ;
    } 
	else
        *exit_code = 1;
}

void	exec_pipe(t_root *root, int32_t *exit_code)
{
	if (root)
	{
		pipeit(root, 0, exit_code);
	}
}
