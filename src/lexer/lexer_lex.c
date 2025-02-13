#include "../../inc/lexer.h"

static t_status	lex_lex(t_lexer *lexer, char **splited_cmd)
{
	bool		tflag;
	t_status	status;

	tflag = true;
	while (*splited_cmd)
	{
		if (minishell_iscmdsep(**splited_cmd))
		{
			status = lex_add_token(lexer, *splited_cmd,
				lex_get_token_type(*splited_cmd));
			tflag = true;
		}
		else if (tflag)
		{
			status = lex_add_token(lexer, *splited_cmd, TTOKEN_COMMAND);
			tflag = false;
		}
		else
			status = lex_add_token(lexer, *splited_cmd,
				lex_get_token_type(*splited_cmd));
		if (status)
			return (status);
		splited_cmd++;
	}
	return (STATUS_SUCCESS);
}

t_status	lexer_lex(t_lexer *lexer)
{
	t_status	status;
	
	status = lex_lex(lexer, lexer->spaced_arr.spaced_cmdline_arr);
	if (status)
		return (status);
	return (STATUS_SUCCESS);
}
