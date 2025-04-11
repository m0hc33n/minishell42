#include "../../inc/minishell.h"

static void	reset_token(t_root *root)
{
	if (root)
	{
		reset_token(root->left);
		reset_token(root->right);
		if (root->tvalue)
		{
			minishell_free((void **)&root->tvalue);
		}
		minishell_free((void **)&root);
	}
}

static void	reset_lexer(t_minishell *minishell)
{
	if (minishell)
	{
		if (minishell->lexer->spaced_cmdline)
			minishell_free((void **)&minishell->lexer->spaced_cmdline);
		if (minishell->root)
		{
			reset_token(minishell->root);
			minishell->root = NULL;
		}
		if (minishell->lexer->splited_cmdline)
			minishell_free((void **)&minishell->lexer->splited_cmdline);
		minishell_memset(minishell->lexer, 0, sizeof(t_lexer));
		minishell_free((void **)&minishell->lexer);
	}
}

static void	free_hdoc_data(t_root *root)
{
	if (root)
	{
		free_hdoc_data(root->left);
		free_hdoc_data(root->right);
		if (root->hd.is_hd)
		{
			minishell_free((void **)&root->hd.filename);
			close(root->hd.fd);
		}
	}
}

void	minishell_reset(t_minishell *minishell)
{
	if (minishell)
	{
		free_hdoc_data(minishell->root);
		if (minishell->lexer)
			reset_lexer(minishell);
		if (minishell->cmdline)
			minishell_free((void **)&minishell->cmdline);
		g_sig_pid = 0;
		if (tcsetattr(STDIN_FILENO, TCSANOW, &minishell->original_termios)
			== -1)
		{
			perror("tcsetattr");
			minishell_cleanup(minishell, STATUS_TERMIOSRES);
		}
	}
}
