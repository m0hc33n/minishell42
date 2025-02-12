#include "../../inc/lexer.h"

static t_status	init_spaced(t_lexer *lexer)
{
	lexer->spaced.sz = cmd_spaced_len(lexer->cmdline); //TODO
	if (lexer->spaced.sz == 0)
		return (STATUS_EMPTYCMD);
	lexer->spaced.spaced_cmdline = (char *)malloc(lexer->spaced.sz);
	if (!lexer->spaced.spaced_cmdline)
		return (STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}	

static bool	cmd_ignore_quotes(char **cmdline, char **spaced_cmdline)
{
	char	quote;

	if (**cmdline == SINGLE_QUOTE || **cmdline == DOUBLE_QUOTE)
	{
		quote = **cmdline;
		*(*spaced_cmdline)++ = *(*cmdline)++;
		while (**cmdline && **cmdline != quote)
			*(*spaced_cmdline)++ = *(*cmdline)++;
		if (!**cmdline)
			return (false);
		return (true);
	}
	return (true);
}

static t_status	cmd_spaced1(char **cmdline, char **spaced_cmdline)
{
	if (!cmd_ignore_quotes(cmdline, spaced_cmdline))
		return (STATUS_SYNTAXERR);
	if (minishell_iscmdsep(*((*cmdline) - 1)))
		*(*spaced_cmdline)++ = SPACE;
	*(*spaced_cmdline)++ = *(*cmdline)++;
	return (STATUS_SUCCESS);
}

static void	cmd_spaced2(char **cmdline, char **spaced_cmdline)
{
	if (minishell_iscmdsep(*((*cmdline) - 1)) && **cmdline == *((*cmdline) - 1))
		*(*spaced_cmdline)++ = *(*cmdline)++;
	else
	{
		if (*((*cmdline) - 1) != SPACE)
			*(*spaced_cmdline)++ = SPACE;
		*(*spaced_cmdline)++ = *(*cmdline)++;
	}
}

t_status	lexer_cmd_spaced(t_lexer *lexer)
{
	t_status	status;
	char		*cmdline;
	char		*spaced_cmdline;

	status = init_spaced(lexer);
	if (status)
		return (status);
	cmdline = lexer->cmdline;
	spaced_cmdline = lexer->spaced.spaced_cmdline;
	while (*cmdline)
	{
		if (!minishell_iscmdsep(*cmdline))
		{
			status = cmd_spaced1(&cmdline, &spaced_cmdline);
			if (status)
				return (status);
		}
		else
			cmd_spaced2(&cmdline, &spaced_cmdline);
	}
	return (STATUS_SUCCESS);
}
