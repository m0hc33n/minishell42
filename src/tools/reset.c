# include "../../inc/minishell.h"

void    reset_token(t_token *token)
{
    t_token *tmp;

    if (token)
    {
        while (token)
        {
            tmp = token->next_token;
            free(token);
            token = tmp;
        }
    }
}

void    reset_lexer(t_lexer *lexer)
{
    if (lexer)
    {
        if (lexer->spaced_arr.spaced_cmdline_arr)
            minishell_free_arr(lexer->spaced_arr.spaced_cmdline_arr);
        if (lexer->spaced.spaced_cmdline)
            free(lexer->spaced.spaced_cmdline);
        if (lexer->token)
            reset_token(lexer->token);
        free(lexer);
    }
}

void    minishell_reset(t_minishell **minishell)
{
    if (minishell && *minishell)
    {
        if ((*minishell)->root)
            free((*minishell)->root);
        if ((*minishell)->lexer)
            reset_lexer((*minishell)->lexer);
        if ((*minishell)->cmdline)
            free((*minishell)->cmdline);
        (*minishell)->exit_code = 0;
    }
}
