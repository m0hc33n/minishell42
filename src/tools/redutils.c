#include "../../inc/tools.h"

bool	minishell_isred(t_root *node)
{
	if (!node)
		return (false);
	if (node->ttype == TTOKEN_HEREDOC
		|| node->ttype == TTOKEN_OUTPUT
		|| node->ttype == TTOKEN_APPEND
		|| node->ttype == TTOKEN_INPUT)
		return (true);
	return (false);
}

t_status	hdoc_keyword_noquotes(char **keyword)
{
	t_token		interim;
	t_status	status;

	interim.tvalue = minishell_strdup(*keyword);
	if (!interim.tvalue)
		return (STATUS_MALLOCERR);
	status = minishell_remove(&interim);
	if (status)
		return (status);
	*keyword = interim.tvalue;
	return (STATUS_SUCCESS);
}
