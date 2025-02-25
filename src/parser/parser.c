#include "../../inc/parser.h"

static t_default_priority	default_priority(t_token_type type)
{
	if (type == TTOKEN_AND_OP || type == TTOKEN_OR_OP)
		return (PRIORITY_CRITICAL);
	else if (type == TTOKEN_REDIRECT
		|| type == TTOKEN_REDIRECT_EOF || type == TTOKEN_REDIRECT_FILE
		|| type == TTOKEN_PIPE)
		return (PRIORITY_HIGH);
	else if (type == TTOKEN_PARENTHESE_OPEN || type == TTOKEN_PARENTHESE_CLOSE)
		return (PRIORITY_IDLE);
	return (PRIORITY_MEDIUM);
}

static void		get_priority(uint32_t i, t_root *nodes)
{
	uint32_t	j;
	uint32_t	counter;

	nodes[i].priority = default_priority(nodes[i].type);
	j = 0;
	counter = 0;
	while (j <= i)
	{
		if (nodes[j].type == TTOKEN_PARENTHESE_OPEN)
			counter += 1;
		if (nodes[j].type == TTOKEN_PARENTHESE_CLOSE)
			counter -= 1;
		j += 1;
	}
	if (nodes[i].type == TTOKEN_PARENTHESE_CLOSE)
		counter += 1;
	nodes[i].priority += counter;
}

static t_root	*parse_tree(t_root *nodes, uint32_t start, uint32_t end)
{
	uint32_t	i;
	uint32_t	prior;

	if (end <= start)
		return (NULL);
	prior = start;
	i = start + 1;
	while (i < end)
	{
		if (nodes[i].priority < nodes[prior].priority)
			prior = i;
		i += 1;
	}
	nodes[prior].left = parse_tree(nodes, start, prior);
	nodes[prior].right = parse_tree(nodes, prior + 1, end);
	return (&nodes[prior]);
}

static void	fill_nodes(t_root *nodes, t_token *token, uint32_t size)
{
	uint32_t	i;

	i = 0;
	while (i < size)
	{
		nodes[i].type = token->ttype;
		nodes[i].value = token->tvalue; 
		get_priority(i, nodes);
		i += 1;
		token = token->next_token;
	}
}

t_status	minishell_parser(t_minishell *minishell)
{
	t_root		*nodes;
	t_token		*token;
	uint32_t	size;

	token = minishell->lexer->token;
	size = minishell->lexer->sztoken;
	nodes = (t_root *)malloc(sizeof(t_root) * size);
	if (!nodes)
		return (STATUS_MALLOCERR);
	fill_nodes(nodes, token, size);
	minishell->root = parse_tree(nodes, 0, size);
	return (STATUS_SUCCESS);
}
