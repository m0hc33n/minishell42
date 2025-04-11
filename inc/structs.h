#ifndef STRUCTS_H
# define STRUCTS_H

# include "minishell.h"

enum e_minichar
{
	CHAR_SINGLE_QUOTE	= 39,
	CHAR_DOUBLE_QUOTE	= 34,
	CHAR_DOLLAR_SIGN	= 36,
	CHAR_AMPERSNAD		= 38,
	CHAR_PAREN_OPEN		= 40,
	CHAR_PAREN_CLOSE	= 41,
	CHAR_ASTERISK		= 42,
	CHAR_DASH			= 45,
	CHAR_GT				= 62,
	CHAR_LT				= 60,
	CHAR_PIPE			= 124
};

typedef enum e_token_type
{
	TTOKEN_COMMAND,
	TTOKEN_ARGUMENT,
	TTOKEN_PIPE,
	TTOKEN_AND_OP,
	TTOKEN_OR_OP,
	TTOKEN_OUTPUT,
	TTOKEN_INPUT,
	TTOKEN_APPEND,
	TTOKEN_HEREDOC,
	TTOKEN_FILE,
	TTOKEN_HEREDOC_KEYWORD,
	TTOKEN_PARENTHESE_OPEN,
	TTOKEN_PARENTHESE_CLOSE,
	TTOKEN_COMMENT,
	TTOKEN_UNKOWN
}	t_token_type;

typedef enum e_status
{
	STATUS_SUCCESS			= 0x000000,
	STATUS_FAILURE			= 0x000001,
	STATUS_MSINITERROR		= 0X000002,
	STATUS_LINITERROR		= 0X000003,
	STATUS_LEXERNULL		= 0x000004,
	STATUS_INITFAIL			= 0x000005,
	STATUS_SYNTAXERR		= 0x000006,
	STATUS_MALLOCERR		= 0x000007,
	STATUS_EMPTYCMD			= 0x000008,
	STATUS_ENVFAILED		= 0x000009,
	STATUS_CMDFAILED		= 0x00000A,
	STATUS_HDOCFAILED		= 0x00000B,
	STATUS_HDOCSIGINT		= 0x00000C,
	STATUS_SIGINIT			= 0x00000D,
	STATUS_CMDNOTFOUND		= 0x00000E,
	STATUS_PATHNOTFOUND		= 0x00000F,
	STATUS_DIRFAILED		= 0x000010,
	STATUS_TERMIOSAVE		= 0X000011,
	STATUS_TERMIOSRES		= 0X000011
}	t_status;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next_key;

	bool			chosen;
}	t_env;

// heredoce handler
typedef struct s_hd
{
	bool	is_hd;
	bool	is_expand;
	char	*filename;
	int32_t	fd;
}	t_hd;

typedef struct s_token
{
	t_token_type	ttype;
	uint32_t		tid;
	char			*tvalue;
	struct s_token	*next_token;
	uint32_t		priority;
	t_hd			hd;
	struct s_token	*right;
	struct s_token	*left;
}	t_token;

typedef t_token	t_root; //same same but different hhh

typedef struct s_lexer
{
	t_token		*token;
	uint32_t	sztoken;
	char		*cmdline;
	char		*spaced_cmdline;
	char		**splited_cmdline;
}	t_lexer;

typedef struct s_minishell
{
	char			*prompt;
	char			*cmdline;
	t_env			*env;
	char			*cwd;
	t_lexer			*lexer;
	t_root			*root;
	int32_t			stdfd[2];
	int32_t			exit_code;
	struct termios	original_termios;
}	t_minishell;

typedef struct s_match
{
	char			*name;
	struct s_match	*next;
}	t_match;

typedef enum e_default_priority
{
	PRIORITY_CRITICAL = 1,
	PRIORITY_HIGH,
	PRIORITY_MEDIUM,
	PRIORITY_IDLE,
	PRIORITY_REMOVE
}	t_default_priority;

typedef struct s_args
{
	char	*exit;
	bool	flag;
	uint8_t	step;
}	t_args;

typedef struct s_chunk
{
	char			*content;
	struct s_chunk	*next;
}	t_chunk;

typedef struct s_result
{
	char	*key;
	char	*value;
	char	*prefix;
	char	*suffix;
	char	*saver;
	char	*result;
	bool	flag[2];
}	t_result;

typedef struct s_ast
{
	bool	before;
	bool	after;
}	t_ast;

typedef struct s_fixe
{
	char		**fixes;
	t_ast		*flags;
	uint32_t	count;
}	t_fixe;

typedef struct s_quotes
{
	bool		*flags;
	uint32_t	fsize;
	uint32_t	i[3];
	char		*fvalue;
}	t_quotes;

typedef struct s_norm_ast
{
	t_match		*ns;
	t_token		*rright;
	bool		first;
	char		*rep;
	t_match		*saver;
	t_token		*cur;
}	t_norm_ast;

typedef struct s_split
{
	char	*s;
	bool	*flags;
	char	c;
	int		count;
	char	**split;
}	t_split;

typedef struct s_norm_pipe
{
	char		**argv;
	t_root		*cmd_node;
	t_status	status;
	char		**envp;
}	t_norm_pipe;

extern pid_t	g_sig_pid;

#endif