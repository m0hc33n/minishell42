#include "../../inc/lexer.h"

static t_status	cmd_spaced_len(char *cmdline, uint64_t *sz)
{
	char	prev_char;

	prev_char = 0;
	while (*cmdline)
	{
		if (minishell_iscmdsep(*cmdline))
		{
			if (!minishell_iscmdsep(prev_char))
				(*sz)++;
			else if (*cmdline != prev_char)
				(*sz)++;
		}
		else
		{
			if (minishell_iscmdsep(prev_char))
				(*sz)++;
		}
		(*sz)++;
		prev_char = *cmdline;
		cmdline++;
	}
	return (STATUS_SUCCESS);
}

static bool	cmd_ignore_quotes(char **cmdline, char **spaced_cmdline)
{
	char	quote;

	if (**cmdline == CHAR_SINGLE_QUOTE || **cmdline == CHAR_DOUBLE_QUOTE)
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

static t_status	cmd_spaced(char **cmdline, char **spaced_cmdline,
					char prev_char, bool cmdissep)
{
	if (!cmdissep)
	{
		if (minishell_iscmdsep(prev_char))
			*(*spaced_cmdline)++ = SPACE;
		if (!cmd_ignore_quotes(cmdline, spaced_cmdline))
			return (STATUS_SYNTAXERR);
		*(*spaced_cmdline)++ = *(*cmdline)++;
	}
	else
	{
		if (minishell_iscmdsep(prev_char)
			&& **cmdline == *((*cmdline) - 1))
			*(*spaced_cmdline)++ = *(*cmdline)++;
		else
		{
			if ((prev_char) != SPACE)
				*(*spaced_cmdline)++ = SPACE;
			*(*spaced_cmdline)++ = *(*cmdline)++;
		}
	}
	return (STATUS_SUCCESS);
}

static t_status	init_spaced(t_lexer *lexer)
{
	cmd_spaced_len(lexer->cmdline, &lexer->spaced.sz);
	if (lexer->spaced.sz == 0)
		return (STATUS_EMPTYCMD);
	lexer->spaced.spaced_cmdline = (char *)minishell_calloc(1, lexer->spaced.sz + 1);
	if (!lexer->spaced.spaced_cmdline)
		return (STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}	

t_status	lexer_cmd_spaced(t_lexer *lexer)
{
	t_status	status;
	char		*cmdline;
	char		*spaced_cmdline;
	char		prev_char;

	status = init_spaced(lexer);
	if (status)
		return (status);
	cmdline = lexer->cmdline;
	spaced_cmdline = lexer->spaced.spaced_cmdline;
	prev_char = 0;
	while (*cmdline)
	{
		if (!minishell_iscmdsep(*cmdline))
		{
			status = cmd_spaced(&cmdline, &spaced_cmdline, prev_char, false);
			if (status)
				return (status);
		}
		else
			cmd_spaced(&cmdline, &spaced_cmdline, prev_char, true);
		prev_char = *cmdline;
	}
	*spaced_cmdline = 0;
	return (STATUS_SUCCESS);
}
