#include "../../inc/executor.h"

static void	exec_redirect_if_needed(t_minishell *minishell, t_root *node,
		int32_t input_fd, int32_t output_fd)
{
	t_root	*rnode;

	rnode = NULL;
	if (minishell_isred(node) || minishell_isred(node->left))
	{
		if (minishell_isred(node))
			rnode = node;
		else
			rnode = node->left;
		exec_redirect(minishell, rnode, input_fd, output_fd);
		exit(minishell->exit_code);
	}
}

static void	setup_input_output(t_root *node, int32_t input_fd,
		int32_t output_fd)
{
	if (input_fd != 0 || node->hd.is_hd)
	{
		if (node->hd.is_hd && dup2(node->hd.fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		else if (dup2(input_fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		if (dup2(output_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(output_fd);
	}
}

static void	pipeit_child(t_minishell *minishell, t_root *node, int32_t input_fd,
		int32_t output_fd)
{
	t_norm_pipe	p;

	signal(SIGQUIT, SIG_DFL);
	exec_redirect_if_needed(minishell, node, input_fd, output_fd);
	p.cmd_node = node->left;
	if (node->ttype == TTOKEN_COMMAND)
		p.cmd_node = node;
	setup_input_output(p.cmd_node, input_fd, output_fd);
	p.status = 0;
	if (node->ttype == TTOKEN_PIPE)
		p.argv = executor_getargs(node->left, minishell, &p.status);
	else
		p.argv = executor_getargs(node, minishell, &p.status);
	if (!p.argv)
		exit(p.status);
	if (minishell_isbuiltin(p.argv[0]))
		exec_builtin(minishell, p.argv);
	else
	{
		p.envp = minishell_getenvp(minishell->env);
		if (p.envp)
			execve(p.argv[0], p.argv, p.envp);
		minishell_free_arr(p.envp);
	}
	(minishell_free((void **)&p.argv), exit(EXIT_FAILURE));
}

static void	handle_parent(t_minishell *minishell, t_root *node,
		int32_t pipe_fd[2], int32_t info[])
{
	int32_t	status;

	if (info[0] != 0)
		close(info[0]);
	if (node->ttype == TTOKEN_PIPE)
		close(pipe_fd[PIPE_WRITE_END]);
	if (node->ttype == TTOKEN_PIPE)
		pipeit(minishell, node->right, pipe_fd[PIPE_READ_END]);
	waitpid(info[1], &status, 0);
	minishell_setstatus(minishell, status);
}

void	pipeit(t_minishell *minishell, t_root *node, int32_t input_fd)
{
	int32_t	pipe_fd[2];
	int32_t	info[2];
	pid_t	pid;

	if (node == NULL)
		return ;
	if (node->ttype == TTOKEN_PIPE && pipe(pipe_fd) == -1)
	{
		perror("pipe");
		minishell_cleanup(minishell, EXIT_FAILURE);
	}
	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		if (node->ttype == TTOKEN_PIPE)
			pipeit_child(minishell, node, input_fd, pipe_fd[PIPE_WRITE_END]);
		else
			pipeit_child(minishell, node, input_fd, minishell->stdfd[1]);
	}
	else if (pid > 0)
	{
		info[0] = input_fd;
		info[1] = pid;
		handle_parent(minishell, node, pipe_fd, info);
	}
}
