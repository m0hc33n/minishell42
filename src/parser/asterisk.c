#include "../../inc/parser.h"

static t_status	add_token(t_token **list, char *s);
static void		free_add_list(t_token *list);
static void		free_fixe(t_fixe *fixes);

t_status	minishell_asterisk(t_token *root, t_token *token) // check if u need to interpret * inside quotes eagle eye baby
{
	DIR				*dirp;
	t_token			*tokens_add;
	struct dirent	*entry;
	t_fixe			*fixe;

	dirp = opendir(".");
	if (!dirp)
		return (STATUS_FAILURE); // custom status pls? :<)
	tokens_add = NULL;
	entry = readdir(dirp);
	while (entry)
	{
		fixe = split_pattern(token->tvalue);
		if (!fixe)
			return (STATUS_MALLOCERR);
		if (matches_pattern(token->tvalue, fixe, entry->d_name))
		{
			if (add_token(&tokens_add, entry->d_name))
				return (STATUS_MALLOCERR);
		}
		entry = readdir(dirp);
	}
	free_fixe(fixe);
	closedir(dirp);
}

static t_status	add_token(t_token **list, char *s)
{
	t_token	*token;
	t_token	*last;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (free_add_list(*list), STATUS_MALLOCERR);
	token->tvalue = minishell_strdup(s); // s is not in heap, read documentation
	if (!token->tvalue)
		return (free(token), free_add_list(*list), STATUS_MALLOCERR); // gadd free_add_list rah makaynach a jemmi
	token->ttype = TTOKEN_ARGUMENT;
	token->right = NULL;
	if (!*list)
		return (*list = token, STATUS_SUCCESS);
	last = *list;
	while (last->right)
		last = last->right;
	last->right = token;
}

static void		free_add_list(t_token *list)
{
	t_token	*next;

	while (list)
	{
		next = list->right;
		free(list);
		list = next;
	}
}

static void		free_fixe(t_fixe *fixe)
{
	minishell_free_arr(fixe->fixes);
	free(fixe->flags);
	free(fixe);
}
