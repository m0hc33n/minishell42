#include "../../inc/minishell.h"

void	reset_token(t_root *root)
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

void	reset_lexer(t_minishell *minishell)
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

void	minishell_reset(t_minishell *minishell)
{
	if (minishell)
	{
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
