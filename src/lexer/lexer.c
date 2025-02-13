#include "minishell.h"

static t_status	lexer_cmd(t_lexer *lexer)
{
	t_status	status;

	if (lexer)
	{
		status = lexer_cmd_spaced(lexer);
		if (status)
			return (status);
		status = lexer_cmd_split(lexer);
		if (status)
			return (status);
		return (STATUS_SUCCESS);
	}
	return (STATUS_LEXERNULL);
} 

static t_status	lexer_init(t_lexer **lexer, t_minishell *ms)
{
	t_status	status;

	if (lexer)
	{
		(*lexer) = (t_lexer *)malloc(sizeof(t_lexer));
		if (!*lexer)
			return (STATUS_MALLOCERR);
		ms->lexer = *lexer;
		(*lexer)->token = NULL;
		(*lexer)->sztoken = 0;
		(*lexer)->cmdline = ms->cmdline;
		(*lexer)->spaced.spaced_cmdline = NULL;
		(*lexer)->spaced.sz = 0;
		status = lexer_cmd(*lexer);
		if (status)
			return (status);
		(*lexer)->current_position = 0;
		(*lexer)->sztoken = 0;
		(*lexer)->token = NULL;
		return (STATUS_SUCCESS);
	}
	return (STATUS_LINITERROR);
}

t_status	minishell_lexer(t_minishell *minishell)
{
	t_lexer		*lexer;
	t_status	status;

	status = lexer_init(&lexer, minishell);
	if (status)
		return (status);
	status = lexer_lex(lexer);
	if (status)
		return (status);
	return (STATUS_SUCCESS);
}
