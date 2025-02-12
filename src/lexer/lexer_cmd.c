#include "../../inc/lexer.h"

t_status	lexer_cmd(t_lexer *lexer)
{
	t_status	status;

	if (lexer)
	{
		status = lexer_cmd_spaced(lexer); //TODO
		if (status)
			return (status);
		status = lexer_cmd_split(lexer); //TODO
		if (status)
			return (status);
	}
	return (STATUS_LEXERCMDERR);
}
