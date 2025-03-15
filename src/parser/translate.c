#include "../../inc/parser.h"

static t_status update(t_token *token, t_env *env, t_args args);
static bool		check_flag(t_token *token);
static void		fix_tree(t_token *token);
static void 	clean_tree(t_token *token);

t_status	minishell_translate(t_token *root, t_env *env, char *str_exitcode)
{
    t_status    status;
	t_args		args;

	args.exit = str_exitcode;
	args.flag = check_flag(root);
	args.step = 0;
	if ((status = update(root, env, args)))
    	return (status);
	args.step = 1;
	if ((status = update(root, env, args)))
    	return (status);
	fix_tree(root);
	clean_tree(root);
    return (STATUS_SUCCESS);
}

static t_status    update(t_token *token, t_env *env, t_args args)
{
    t_status    status;
	char		*temp;

    if (token)
    {
		if (minishell_strchr(token->tvalue, '$') || minishell_strchr(token->tvalue, '*'))
		{
			if ((status = minishell_interpret(token, env, args)))
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
		args.flag = check_flag(token);
        if ((status = update(token->right, env, args)))
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
			free(token->left->tvalue);
            free(token->left);
            token->left = NULL;
        }
        if (token->right && (token->right->ttype == TTOKEN_PARENTHESE_CLOSE 
        || token->right->ttype == TTOKEN_PARENTHESE_OPEN))
        {
			free(token->right->tvalue);
            free(token->right);
            token->right = NULL;
        }
        clean_tree(token->left);
        clean_tree(token->right);
    }
}
