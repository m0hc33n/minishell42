#include "../../inc/minishell.h"

static void	exec_pipe(t_minishell *minishell, t_root *root)
{
	pipeit(minishell, root, STDIN_FILENO);
}

static void	executor_exec(t_minishell *minishell, t_root *root)
{
	if (root)
	{
		if (root->ttype == TTOKEN_AND_OP)
		{
			executor_exec(minishell, root->left);
			if (!minishell->exit_code)
				executor_exec(minishell, root->right);
		}
		else if (root->ttype == TTOKEN_OR_OP)
		{
			executor_exec(minishell, root->left);
			if (minishell->exit_code)
				executor_exec(minishell, root->right);
		}
		else if (root->ttype == TTOKEN_PIPE)
			exec_pipe(minishell, root);
		else if (minishell_isred(root))
			exec_redirect(minishell, root, STDIN_FILENO, STDOUT_FILENO);
		else if (root->ttype == TTOKEN_COMMAND)
			exec_cmd(minishell, root);
	}
}

t_status	minishell_executor(t_minishell *minishell)
{
	t_status	status;

	status = 0;
	executor_handle_hdoc(minishell->root, &status);
	if (status)
	{
		if (status == STATUS_HDOCSIGINT)
			minishell->exit_code = 130;
		if (minishell->root->left->hd.is_hd)
		{
			minishell_free((void **)&minishell->root->left->hd.filename);
			close(minishell->root->left->hd.fd);
		}
		return (status);
	}
	executor_exec(minishell, minishell->root);
	return (STATUS_SUCCESS);
}
