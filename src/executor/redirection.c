#include "../../inc/executor.h"

static bool	expand_hdoc_in(t_root *cmd_node, t_env *env, int32_t exit_code)
{
	char	*fdata;
	char	*expanded;
	int32_t	fd;
	t_args	args;

	fdata = minishell_readfile(cmd_node->hd.filename);
	if (!fdata)
		return (false);
	args.exit = minishell_i32tostr(exit_code);
	expanded = fdata;
	if (cmd_node->hd.is_expand)
	{
		expanded = minishell_expand(fdata, env, args);
		minishell_free((void **)&fdata);
	}
	fd = open(cmd_node->hd.filename, O_CREAT | O_RDWR | O_TRUNC);
	if (fd == -1)
		return (false);
	write(fd, expanded, minishell_strlen(expanded));
	minishell_free((void **)&expanded);
	return (close(fd), unlink(cmd_node->hd.filename), true);
}

static t_status	handle_ioa(t_root *node, t_root *cmd_node, int32_t input_fd,
		int32_t output_fd)
{
	while (minishell_isred(node))
	{
		if (node->ttype == TTOKEN_OUTPUT)
			return (redirect_output(node, output_fd));
		else if (node->ttype == TTOKEN_APPEND)
			return (redirect_append(node, output_fd));
		else if (node->ttype == TTOKEN_INPUT)
			return (redirect_input(node, input_fd));
		else if (node->ttype == TTOKEN_HEREDOC)
			return (redirect_hdoc(cmd_node, input_fd));
		node = node->right;
		while (node && (!minishell_isred(node)))
			node = node->right;
	}
	return (STATUS_SUCCESS);
}

static void	redirect_clean(int32_t fds[], int32_t input_fd, int32_t output_fd)
{
	dup2(fds[0], input_fd);
	dup2(fds[1], output_fd);
	close(fds[0]);
	close(fds[1]);
}

void	exec_redirect(t_minishell *minishell, t_root *node, int32_t input_fd,
		int32_t output_fd)
{
	t_root	*cmd_node;
	int32_t	bkpfd[2];
	bool	tflag;

	bkpfd[0] = dup(input_fd);
	bkpfd[1] = dup(output_fd);
	if (bkpfd[0] == -1 || bkpfd[1] == -1)
	{
		perror("exec_redirect");
		return ;
	}
	cmd_node = node->left;
	tflag = true;
	if (handle_ioa(node, cmd_node, input_fd, output_fd))
		return ;
	if (cmd_node->hd.is_hd)
		tflag = expand_hdoc_in(cmd_node, minishell->env, minishell->exit_code);
	if (tflag)
		exec_cmd(minishell, cmd_node);
	if (cmd_node->hd.is_hd && setbool(&cmd_node->hd.is_hd, false))
	{
		minishell_free((void **)&cmd_node->hd.filename);
		close(cmd_node->hd.fd);
	}
	redirect_clean(bkpfd, input_fd, output_fd);
}
