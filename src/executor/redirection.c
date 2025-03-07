#include "../../inc/executor.h"

static bool	expand_hdoc_in(char *filename, t_env *env)
{
	char	*fdata;
	char	*expanded;
	int32_t	fd;

	fdata = minishell_readfile(filename);
	if (!fdata)
		return (false);
	expanded = minishell_expand(fdata, env); // NO WORKING
	free(fdata);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (false);
	write(fd, expanded, minishell_strlen(expanded));
	close(fd);
	unlink(filename);
	return (true);
}

static void	handle_ioa(t_root *node, t_root *cmd_node,
				int32_t input_fd, int32_t output_fd)
{
	while (minishell_isred(node))
	{
		if (node->ttype == TTOKEN_OUTPUT)
			redirect_output(node, output_fd);
		else if (node->ttype == TTOKEN_APPEND)
			redirect_append(node, output_fd);
		else if (node->ttype == TTOKEN_INPUT)
			redirect_input(node, input_fd);
		else if (node->ttype == TTOKEN_HEREDOC)
			redirect_hdoc(cmd_node, input_fd);
		node = node->right;
		while (node && (!minishell_isred(node)))
			node = node->right;
	}
}

void		exec_redirect(t_minishell *minishell, t_root *node,
				int32_t input_fd, int32_t output_fd)
{
	t_root	*cmd_node;
	int32_t	bkpfd[2];
	bool	tflag;

	bkpfd[0] = dup(input_fd);
	bkpfd[1] = dup(output_fd);
	cmd_node = node->left;
	tflag = true;
	handle_ioa(node, cmd_node, input_fd, output_fd);
	if (cmd_node->hd.is_hd)
		tflag = expand_hdoc_in(cmd_node->hd.filename, minishell->env);
	if (tflag)
		exec_cmd(minishell, cmd_node, input_fd, output_fd); // TOTEST: did u use in/out fd?
	if (cmd_node->hd.is_hd)
		close(cmd_node->hd.fd); // TOTEST
	dup2(bkpfd[0], input_fd);
	dup2(bkpfd[1], output_fd);
}
