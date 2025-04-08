#include "../../inc/parser.h"

static t_status	add_name(t_match **names, char *s);
static void		free_mem(t_match *names, t_fixe *fixe);
static t_status	add_to_tree(t_token *token, t_match *names);
static t_status	add_name_to_tree(t_norm_ast *local);

t_status	minishell_asterisk(t_token *token, bool *asterisk)
{
	t_match			*names;
	t_fixe			*fixe;
	DIR				*dirp;
	struct dirent	*entry;

	names = NULL;
	fixe = minishell_analyse(token->tvalue, asterisk);
	if (!fixe)
		return (STATUS_MALLOCERR);
	dirp = opendir(".");
	if (!dirp)
		return (free_mem(NULL, fixe), STATUS_FAILURE);
	entry = readdir(dirp);
	while (entry)
	{
		if (entry->d_name[0] != '.' && minishell_matcher(fixe, entry->d_name))
		{
			if (add_name(&names, entry->d_name))
				return (free_mem(names, fixe),
					closedir(dirp), STATUS_MALLOCERR);
		}
		entry = readdir(dirp);
	}
	return (free_mem(NULL, fixe), closedir(dirp), add_to_tree(token, names));
}

static t_status	add_name(t_match **names, char *s)
{
	t_match	*match;
	t_match	*last;

	match = (t_match *)malloc(sizeof(t_match));
	if (!match)
		return (STATUS_MALLOCERR);
	match->name = minishell_strdup(s);
	if (!match->name)
		return (minishell_free((void **)&match), STATUS_MALLOCERR);
	match->next = NULL;
	if (!*names)
		return (*names = match, STATUS_SUCCESS);
	last = *names;
	while (last->next)
		last = last->next;
	last->next = match;
	return (STATUS_SUCCESS);
}

static t_status	add_to_tree(t_token *token, t_match *names)
{
	t_norm_ast	local;
	t_status	status;

	local.names = names;
	local.rright = token->right;
	local.first = true;
	local.saver = names;
	local.cur = token;
	while (local.names)
	{
		status = add_name_to_tree(&local);
		if (status)
			return (status);
	}
	return (free_mem(local.saver, NULL), STATUS_SUCCESS);
}

static t_status	add_name_to_tree(t_norm_ast *local)
{
	local->rep = minishell_strdup(local->names->name);
	if (!local->rep)
		return (free_mem(local->names, NULL), STATUS_MALLOCERR);
	if (local->first)
	{
		minishell_free((void **)&local->cur->tvalue);
		local->first = false;
	}
	local->cur->tvalue = local->rep;
	local->cur->ttype = TTOKEN_ARGUMENT;
	local->names = local->names->next;
	if (local->names)
	{
		local->cur->right = (t_token *)malloc(sizeof(t_token));
		if (!local->cur->right)
		{
			local->cur->right = local->rright;
			return (free_mem(local->names, NULL), STATUS_MALLOCERR);
		}
		local->cur = local->cur->right;
		local->cur->left = NULL;
	}
	else
		local->cur->right = local->rright;
	return (STATUS_SUCCESS);
}

static void	free_mem(t_match *names, t_fixe *fixe)
{
	t_match	*next;

	while (names)
	{
		next = names->next;
		minishell_free((void **)&names->name);
		minishell_free((void **)&names);
		names = next;
	}
	if (fixe)
	{
		minishell_free_arr(fixe->fixes);
		minishell_free((void **)&fixe->flags);
		minishell_free((void **)&fixe);
	}
}
