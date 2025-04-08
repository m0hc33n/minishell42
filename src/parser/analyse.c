#include "../../inc/parser.h"

static void	fill_flags(char *pattern, t_fixe *fixe);

t_fixe	*minishell_analyse(char *pattern, bool *asterisk)
{
	t_fixe		*fixe;

	fixe = (t_fixe *)malloc(sizeof(t_fixe));
	if (!fixe)
		return (NULL);
	fixe->fixes = minishell_split(pattern, '*', asterisk);
	if (!fixe->fixes)
		return (minishell_free((void **)&fixe), NULL);
	fixe->count = 0;
	while (fixe->fixes[fixe->count])
		fixe->count += 1;
	fixe->flags = (t_ast *)malloc(sizeof(t_ast) * fixe->count);
	if (!fixe->flags)
		return (minishell_free((void **)&fixe->fixes), minishell_free((void **)&fixe), NULL);
	fill_flags(pattern, fixe);
	return (fixe);
}

static void	fill_flags(char *pattern, t_fixe *fixe)
{
	uint32_t	i;
	uint32_t	c;
	t_ast 		*flag;

	i = 0;
	c = 0;
	while (c < fixe->count)
	{
		flag = fixe->flags + c;
		flag->before = false;
		if (pattern[i] == '*')
		{
			flag->before = true;
			while (pattern[i] && pattern[i] == '*')
				i += 1;
		}
		while (pattern[i] && pattern[i] != '*')
			i += 1;
		flag->after = false;
		if (pattern[i] == '*')
			flag->after = true;
		c += 1;
	}
}
