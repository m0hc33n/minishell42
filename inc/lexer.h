#ifndef LEXER_H
# define LEXER_H

#include "tools.h"

// CMD AND WORD SEPARATORS
# define METACHARACTER "|&()<> "
# define CMDSEP "|&()><"

enum e_char_cmdsep
{
	CHAR_SINGLE_QUOTE	= 39,
	CHAR_DOUBLE_QUOTE	= 34,
	CHAR_DOLLAR_SIGN	= 36,
	CHAR_AMPERSNAD		= 38,
	CHAR_PAREN_OPEN		= 40,
	CHAR_PAREN_CLOSE	= 41,
	CHAR_DASH			= 45,
	CHAR_GT				= 62,
	CHAR_LT				= 60,
	CHAR_PIPE			= 124
};

typedef enum e_token_type
{
    TTOKEN_COMMAND,
    TTOKEN_OPTION,
    TTOKEN_ARGUMENT,
    TTOKEN_STRING,
    TTOKEN_PIPE,
    TTOKEN_AND_OP,
    TTOKEN_OR_OP,
    TTOKEN_REDIRECT,
	TTOKEN_PARENTHESE_OPEN,
	TTOKEN_PARENTHESE_CLOSE,
    TTOKEN_COMMENT,
    TTOKEN_VARIABLE,
    TTOKEN_UNKOWN
}	t_token_type;

typedef struct s_token
{
	t_token_type	ttype;
	uint32_t		tid;
	char			*tvalue;
	struct s_token	*next_token;
}	t_token;

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
    int32_t         current_position;
}	t_lexer;

t_status		minishell_lexer(t_minishell *minishell);
t_status		lexer_cmd_spaced(t_lexer *lexer);
t_status    	lexer_cmd_split(t_lexer *lexer);
t_status		lexer_lex(t_lexer *lexer);
t_status		lex_add_token(t_lexer *lexer, char *tvalue, t_token_type ttype);
t_token_type	lex_get_token_type(char *token_vlaue);

#endif