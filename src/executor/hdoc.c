#include "../../inc/executor.h"

static char	*insert_newline(char *buffer)
{
	char	*buffer_nl;
	int32_t	i;

	if (!buffer)
		return (NULL);
	buffer_nl = (char *)malloc(minishell_strlen(buffer) + 2);
	i = 0;
	while (buffer[i])
	{
		buffer_nl[i] = buffer[i];
		i++;
	}
	buffer_nl[i] = NEWLINE;
	buffer_nl[++i] = 0;
	free(buffer);
	return (buffer_nl);
}

static void	hdoc_input(int32_t fd, char *keyword)
{
	char	*buffer;
	char	*buffer_nl;

	signal(SIGINT, hdoc_sigint);
	while (true)
	{
		buffer = readline("> ");
		if (minishell_strequal(keyword, buffer))
			break ;
		buffer_nl = insert_newline(buffer);
		write(fd, buffer_nl, minishell_strlen(buffer_nl));
		free(buffer_nl);
	}
	close(fd);
	exit(STATUS_SUCCESS);
}

static t_status	handle_hdoc(t_root *cmd_node, t_root *hdoc_node)
{
	int32_t	fd;
	int32_t	status;
	char	*keyword;

	if (hdoc_node->right->ttype == TTOKEN_HEREDOC_KEYWORD)
		keyword = hdoc_node->right->tvalue;
	else
		keyword = hdoc_node->right->left->tvalue;
	cmd_node->hd.filename = minishell_generate_filename();
	if (cmd_node->hd.fd)
		close(cmd_node->hd.fd);	// TOTEST;
	cmd_node->hd.fd = open(cmd_node->hd.filename, O_CREAT | O_RDONLY, 0644);
	if (!cmd_node->hd.filename || cmd_node->hd.fd == -1)
        return(STATUS_HDOCFAILED);
	if (fork() == CHILD_PROCESS)
	{
		fd = open(cmd_node->hd.filename, O_RDWR);
		if (fd == -1)
			exit(STATUS_HDOCFAILED);
		hdoc_input(fd, keyword);
	}
	wait(&status);
	return (WEXITSTATUS(status));
}

void	executor_handle_hdoc(t_root *root, t_status *status)
{
	t_root		*cmd_node;

	if (!root || *status)
		return ;
	if (minishell_isred(root))
	{
		cmd_node = root->left;
		while (root && minishell_isred(root))
		{
			if (root->ttype == TTOKEN_HEREDOC)
			{	
				cmd_node->hd.is_hd = true;
				*status = handle_hdoc(cmd_node, root);
				if (*status)
					return ;
			}
			root = root->right;
		}
	}
    executor_handle_hdoc(root->left, status);
    executor_handle_hdoc(root->right, status);
}
