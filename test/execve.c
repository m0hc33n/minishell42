#include <stdio.h>
#include <unistd.h>

#include "../inc/executor.h"

static int32_t	exec_pipe_left(t_root *root, int pipefd[])
{
	pid_t	pid;
	char	**argv;
	int32_t	status;

	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		argv = cmd_get_argv(root); //TODO
		execve(argv[0], argv, NULL);
		perror("execve");
	}
	wait(&status);
	return (status);
}

static int32_t	exec_pipe_right(t_root *root, int pipefd[])
{
	pid_t	pid;
	char	**argv;
	int32_t	status;

	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		argv = cmd_get_argv(root); //TODO
		execve(argv[0], argv, NULL);
		perror("execve");
	}
	wait(&status);
	return (status);
}

void	exec_pipe(t_root *root, int32_t *exit_code)
{
	int		pipefd[2];

	if (pipe(pipefd) == -1)
		return ;
	exit_code = exec_pipe_left(root->left, pipefd);
	if (!exit_code)
		exit_code = exec_pipe_right(root->right, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
}


int main(int ac, char **av, char **env)
{
	
}
