# include "../../inc/minishell.h"

void    reset_token(t_root *root)
{
    if (root)
    {
        reset_token(root->left);
        reset_token(root->right);
        if (root->tvalue)
		{
        	free(root->tvalue);
			root->tvalue = NULL;
		}
		free(root);
		root = NULL;
	}
}

void    reset_lexer(t_minishell *minishell)
{
    if (minishell)
    {
        if (minishell->lexer->spaced.spaced_cmdline)
            free(minishell->lexer->spaced.spaced_cmdline);
        if (minishell->root)
            reset_token(minishell->root);
		minishell_memset(minishell->lexer, 0, sizeof(t_lexer));
        free(minishell->lexer);
		minishell->lexer = NULL;
    }
}

void    minishell_reset(t_minishell *minishell)
{
    if (minishell)
    {
        if (minishell->lexer)
            reset_lexer(minishell);
        if (minishell->cmdline)
           free(minishell->cmdline);
    }
}
