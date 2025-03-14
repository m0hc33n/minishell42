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
	STATUS_DIRFAILED		= 0x000010
}	t_status;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next_key;
}	t_env;

// heredoce handler
typedef struct s_hd
{
	bool	is_hd;
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
	t_hd			hd;		// do u use it yet?
	struct s_token	*right;
	struct s_token	*left;
}	t_token;

typedef t_token	t_root; //same same but different hhh

/**/
typedef struct s_spaced_cmdline
{
    char        *spaced_cmdline;
    uint64_t    sz;
}   t_spaced_cmdline;

typedef struct s_spaced_cmdline_arr
{
	char		**spaced_cmdline_arr;
	uint64_t	sz;
}	t_spaced_cmdline_arr;


typedef struct s_lexer
{
	t_token					*token;
	uint32_t				sztoken;
    char           			*cmdline;
    t_spaced_cmdline		spaced;
    t_spaced_cmdline_arr	spaced_arr;
}	t_lexer;

// typedef struct s_envr
// {
// 	char			*key;
// 	char			*value;
// 	struct s_env	*next_key;
// }	t_envr;

typedef struct s_minishell
{
	char	*prompt;
	char	*cmdline;
	t_env	*env;
	t_lexer	*lexer;
	t_root	*root;
	int32_t	stdfd[2];
	int32_t	exit_code;
	
}	t_minishell;

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

#endif