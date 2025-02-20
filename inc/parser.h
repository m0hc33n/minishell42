#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

// typedef enum e_node_type
// {
// 	TNODE_COMMAND,
// 	TNODE_ARG,
// 	TNODE_PIPE,
// 	TNODE_AND,
// 	TNODE_OR,
// 	TNODE_IRED,
// 	TNODE_ORED,
// 	TNODE_IRED_OVER,
// 	TNODE_ORED_OVER
// }	t_node_type;

typedef enum e_default_priority
{
	PRIORITY_CRITICAL = 1,
	PRIORITY_HIGHT,
	PRIORITY_MEDIUM,
	PRIORITY_IDLE
}	t_default_priority;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next_key
}	t_env;

typedef struct s_root
{
	t_token_type		type;
	char				*value;
	uint32_t			priority;
	struct s_root		*right;
	struct s_root		*left;
} t_root;

t_status	minishell_parser(t_minishell *minishell);

#endif