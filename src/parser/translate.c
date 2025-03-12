#include "../../inc/parser.h"

static t_status update(t_token *token, t_env *env, bool flag, uint8_t step);
static bool		check_flag(t_token *token);
static void		fix_tree(t_token *token);
static void 	clean_tree(t_token *token);

t_status	minishell_translate(t_token *root, t_env *env)
{
    t_status    status;
	bool		flag;

	flag = check_flag(root);
	if ((status = update(root, env, flag, 0)))
    	return (status);
	if ((status = update(root, env, flag, 1)))
    	return (status);
	fix_tree(root);
	clean_tree(root);
    return (STATUS_SUCCESS);
}

static t_status    update(t_token *token, t_env *env, bool flag, uint8_t step)
{
    t_status    status;
	char		*temp;

    if (token)
    {
		if (minishell_strchr(token->tvalue, '$') || minishell_strchr(token->tvalue, '*'))
		{
			if ((status = minishell_interpret(token, env, flag, step)))
				return (status);
		}
		if (token->ttype == TTOKEN_COMMAND && !minishell_isbuiltin(token->tvalue))
		{
			temp = minishell_getpath(env, token->tvalue);
			if (!temp)
				return (STATUS_CMDFAILED);
			free(token->tvalue);
			token->tvalue = temp;
		}
        if ((status = update(token->right, env, check_flag(token), step)))
            return (status);
        return (STATUS_SUCCESS);
    }
    return (STATUS_SUCCESS);
}

static bool		check_flag(t_token *token)
{
	if (token->ttype == TTOKEN_COMMAND && minishell_strequal(token->tvalue, EXPORT))
		return (false);
	return (true);
}

static void		fix_tree(t_token *token)
{
	t_token	*rright;

	if (token)
	{
		if (token->right && !token->right->tvalue)
		{
			rright = token->right->right;
			free(token->right);
			token->right = rright;
		}
		fix_tree(token->right);
	}
}

static void	clean_tree(t_token *token) // just removes parenthesis'
{
    if (token)
    {
        if (token->left && (token->left->ttype == TTOKEN_PARENTHESE_CLOSE 
		|| token->left->ttype == TTOKEN_PARENTHESE_OPEN))
        {
            free(token->left);
            token->left = NULL;
        }
        if (token->right && (token->right->ttype == TTOKEN_PARENTHESE_CLOSE 
        || token->right->ttype == TTOKEN_PARENTHESE_OPEN))
        {
            free(token->right);
            token->right = NULL;
        }
        clean_tree(token->left);
        clean_tree(token->right);
    }
}
