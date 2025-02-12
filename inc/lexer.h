#ifndef LEXER_H
# define LEXER_H

#include "tools.h"

// CMD AND WORD SEPARATORS
# define METACHARACTER "|&()<> "
# define CMDSEP "|&()><"


# define SINGLE_QUOTE 39
# define DOUBLE_QUOTE 34

typedef enum e_token_type
{
    COMMAND,
    OPTION,
    ARGUMENT,
    PIPE,
    CHAR_AND_OP = '&',
    CHAR_OR_OP = '|',
    REDIRECT,
    CHAR_COMMENT = '#',
    STRING,
    VARIABLE,
    DOLARCOMMAND,
    EOF_TOK
}	t_token_type;

typedef enum e_token_state
{
    SINGLE_QUOTE_OPEN,
    SINGLE_QUOTE_CLOSE,
    DOUBLE_QOUTE_OPEN,
    DOUBLE_QOUTE_CLOSE,
    LEFT_PARENTHESE_OPEN,
    RIHGT_PARENTHESE_CLOSE,
}   t_token_state;

typedef struct s_token
{
	t_token_type	ttype;
	char			*value;
    int32_t         position;
	struct s_token	*next_token;
}	t_token;

/**/
typedef struct s_spaced_cmd
{
    char        *spaced_cmdline;
    uint32_t    *sz;
}   t_spaced_cmd;

typedef struct s_lexer
{
	t_token			*token;
	uint32_t		sztoken;
    char            *cmdline;
    t_spaced_cmd	spaced;
    char            **spaced_cmdline_arr;
    int32_t         current_position;
}	t_lexer;

t_status	minishell_lexer(t_minishell *minishell);
t_status    lexer_cmd(t_lexer *lexer);
t_status	lexer_cmd_spaced(t_lexer *lexer);
t_status    lexer_cmd_split(t_lexer *lexer);

#endif