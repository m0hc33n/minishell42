#include "../../inc/executor.h"

static void redirect_heredoc(t_minishell *minishell, t_root *node,
				int32_t *hdfd, bool *fflag)
{
	char	buffer[HRD_BUFFER_SIZE];
	int32_t	bytes_read;

	bytes_read = HRD_BUFFER_SIZE;
	*hdfd = open(TMP, __O_TMPFILE | O_RDWR | O_TRUNC, 0644);
	if (*hdfd == -1)
	{
		perror("Error opening file");
		return ;
	}
	while (bytes_read > 0)
	{
		minishell_memset(&buffer, 0, HRD_BUFFER_SIZE);
		bytes_read = read(STDIN_FILENO, &buffer, HRD_BUFFER_SIZE);
		if (!minishell_strequal(buffer, node->tvalue))
		{
			dup2(*hdfd, minishell->stdfd[0]);
			*fflag = true;
			return ;
		}
		else
			write(*hdfd, &buffer, bytes_read);
	}
}

static void	handle_hd(t_minishell *minishell, t_root *node,
				int32_t *hdfd, bool *fflag)
{
	t_root	*hd_node;

	while (node && node->ttype != TTOKEN_HEREDOC)
		node = node->right;
	if (node && node->ttype == TTOKEN_HEREDOC)
	{
		if (*hdfd != -1)
			close(*hdfd);
		if (node->right->ttype == TTOKEN_HEREDOC)
			hd_node = node->right->left;
		else
			hd_node = node->right;
		redirect_heredoc(minishell, hd_node, hdfd, fflag);
		handle_hd(minishell, node->right, hdfd, fflag);
	}
}

static void	handle_ioa(t_root *node, int32_t input_fd, int32_t output_fd)
{
	while (minishell_isred(node))
	{
		if (node->ttype == TTOKEN_OUTPUT)
			redirect_output(node, output_fd);
		else if (node->ttype == TTOKEN_APPEND)
			redirect_append(node, output_fd);
		else if (node->ttype == TTOKEN_INPUT)
			redirect_input(node, input_fd);
		node = node->right;
		while (node && (!minishell_isred(node)) && node->ttype != TTOKEN_HEREDOC)
			node = node->right;
	}
}

void		exec_redirect(t_minishell *minishell, t_root *node,
				int32_t input_fd, int32_t output_fd)
{
	t_root	*cmd_node;
	int32_t	bkpfd[2];
	int32_t	hdfd;
	bool	fflag;

	bkpfd[0] = dup(input_fd);
	bkpfd[1] = dup(output_fd);
	fflag = false;
	hdfd = -1;
	cmd_node = node->left;
	handle_hd(minishell, node, &hdfd, &fflag);
	handle_ioa(node, input_fd, output_fd);
	exec_cmd(minishell, cmd_node, output_fd);
	if (fflag)
	{
		close(hdfd);
		dup2(minishell->stdfd[0], STDIN_FILENO);
	}
	dup2(bkpfd[1], output_fd);
	dup2(bkpfd[0], input_fd);
}
