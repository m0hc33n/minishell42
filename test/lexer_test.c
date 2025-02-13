#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <readline/readline.h>


# define CMDSEP "|&()><"


///////////

bool	minishell_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return (true);
		s++;
	}
	return (false);
}

bool	minishell_isspace(int c)
{
	return (c == 32);
}


uint32_t	minishell_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (*(s + len))
		len++;
	return (len);
}

bool	minishell_iscmdsep(int c)
{
	if (minishell_strchr(CMDSEP, c))
		return (true);
	return (false);
}

void	*minishell_memset(void *b, int c, size_t len)
{
	while (len--)
	{
		*(unsigned char *)(b + len) = (unsigned char)c;
	}
	return (b);
}

void	*minishell_calloc(size_t count, size_t size)
{
	void	*ptr;

	if ((int)count < 0 && (int)size < 0)
		return (0);
	ptr = malloc(count * size);
	if (!ptr)
		return (0);
	minishell_memset(ptr, 0, count * size);
	return (ptr);
}

uint64_t	minishell_strlcpy(char *dst, const char *src, size_t dstsize)
{
	uint64_t	i;
	uint64_t	src_len;
	char	*s;

	i = 0;
	s = (char *)src;
	src_len = minishell_strlen(src);
	while (dstsize != 0 && i < dstsize - 1 && *(s + i))
	{
		*(dst + i) = *(s + i);
		i++;
	}
	if (dstsize != 0)
		*(dst + i) = '\0';
	return (src_len);
}

////////////

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
	TTOKEN_REDIRECT_FILE,
	TTOKEN_REDIRECT_EOF,
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
}	t_lexer;;

typedef struct s_minishell
{
	char	*prompt;
	char	*cmdline;
	t_lexer	*lexer;
}	t_minishell;

typedef enum e_status
{
	STATUS_SUCCESS			= 0x000000,
	STATUS_FAILURE			= 0x000001,
	STATUS_LINITERROR		= 0X000002,
	STATUS_MSINITERROR		= 0X000003,
	STATUS_INITFAIL			= 0x000010,
	STATUS_SYNTAXERR		= 0x000011,
	STATUS_MALLOCERR		= 0x000012,
	STATUS_LEXERCMDERR		= 0x000013,
	STATUS_EMPTYCMD			= 0x000014
}	t_status;


static t_status	cmd_spaced_len(char *cmdline, uint64_t *sz)
{
	while (*cmdline)
	{
		if (minishell_iscmdsep(*cmdline))
		{
			if (!minishell_iscmdsep(*(cmdline - 1)))
				(*sz)++;
			else if (*cmdline != *(cmdline - 1))
				(*sz)++;
		}
		else
		{
			if (minishell_iscmdsep(*(cmdline - 1)))
				(*sz)++;
		}
		(*sz)++;
		cmdline++;
	}
	return (STATUS_SUCCESS);
}

static bool	cmd_ignore_quotes(char **cmdline, char **spaced_cmdline)
{
	char	quote;

	if (**cmdline == CHAR_SINGLE_QUOTE || **cmdline == CHAR_DOUBLE_QUOTE)
	{
		quote = **cmdline;
		*(*spaced_cmdline)++ = *(*cmdline)++;
		while (**cmdline && **cmdline != quote)
			*(*spaced_cmdline)++ = *(*cmdline)++;
		if (!**cmdline)
			return (false);
		return (true);
	}
	return (true);
}

static t_status	cmd_spaced(char **cmdline, char **spaced_cmdline, bool cmdissep)
{
	if (!cmdissep)
	{
		if (minishell_iscmdsep(*((*cmdline) - 1)))
			*(*spaced_cmdline)++ = SPACE;
		if (!cmd_ignore_quotes(cmdline, spaced_cmdline))
			return (STATUS_SYNTAXERR);
		*(*spaced_cmdline)++ = *(*cmdline)++;
	}
	else
	{
		if (minishell_iscmdsep(*((*cmdline) - 1))
			&& **cmdline == *((*cmdline) - 1))
			*(*spaced_cmdline)++ = *(*cmdline)++;
		else
		{
			if (*((*cmdline) - 1) != SPACE)
				*(*spaced_cmdline)++ = SPACE;
			*(*spaced_cmdline)++ = *(*cmdline)++;
		}
	}
	return (STATUS_SUCCESS);
}

static t_status	init_spaced(t_lexer *lexer)
{
	cmd_spaced_len(lexer->cmdline, &lexer->spaced.sz);
	if (lexer->spaced.sz == 0)
		return (STATUS_EMPTYCMD);
	lexer->spaced.spaced_cmdline = (char *)malloc(lexer->spaced.sz);
	if (!lexer->spaced.spaced_cmdline)
		return (STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}	

t_status	lexer_cmd_spaced(t_lexer *lexer)
{
	t_status	status;
	char		*cmdline;
	char		*spaced_cmdline;

	status = init_spaced(lexer);
	if (status)
		return (status);
	cmdline = lexer->cmdline;
	spaced_cmdline = lexer->spaced.spaced_cmdline;
	while (*cmdline)
	{
		if (!minishell_iscmdsep(*cmdline))
		{
			status = cmd_spaced(&cmdline, &spaced_cmdline, false);
			if (status)
				return (status);
		}
		else
			cmd_spaced(&cmdline, &spaced_cmdline, true);
	}
	*spaced_cmdline = 0;
	return (STATUS_SUCCESS);
}



static void	cmd_split_size(const char *s, uint64_t *count)
{
	bool	flag;
	char	quote;

	flag = false;
	*count = 1;
	while (*s)
	{
		if (*s == CHAR_SINGLE_QUOTE || *s == CHAR_DOUBLE_QUOTE)
		{
			quote = *s++;
			while (*s != quote)
				s++;
			s++;
			(*count)++;
			continue ;
		}
		if (*s != SPACE && !flag)
		{
			(*count)++;
			flag = true;
		}
		else if (*s == SPACE)
			flag = false;
		s++;
	}
}

void	cmd_split_quoted(char **spaced, t_lexer *lexer,
			uint32_t element)
{
	char		quote;
	uint64_t	len;

	len = 0;
	quote = *(*spaced)++;
	while (*((*spaced) + len) != quote)
		len++;
	len += 2;
	lexer->spaced_arr.spaced_cmdline_arr[element]
		= (char *)minishell_calloc(len + 1, 1);
	minishell_strlcpy(lexer->spaced_arr.spaced_cmdline_arr[element],
		*(spaced) - 1, len + 1);
	*spaced += len;
}

static t_status	cmd_split_word(char **spaced, t_lexer *lexer,
	uint32_t element)
{
	uint64_t	len;
	size_t		i;

	len = 0;
	if (**spaced == CHAR_SINGLE_QUOTE || **spaced == CHAR_DOUBLE_QUOTE)
	{
		cmd_split_quoted(spaced, lexer, element);
		return (STATUS_SUCCESS);
	}
	while (*((*spaced) + len) && *((*spaced) + len) != SPACE)
		len++;
	lexer->spaced_arr.spaced_cmdline_arr[element]
		= (char *)minishell_calloc(len + 1, 1);
	if (!lexer->spaced_arr.spaced_cmdline_arr[element])
		return (STATUS_MALLOCERR);
	minishell_strlcpy(lexer->spaced_arr.spaced_cmdline_arr[element],
		*spaced, len + 1);
	*spaced += len;
	return (STATUS_SUCCESS);
}

static t_status	init_split(t_lexer *lexer)
{
	cmd_split_size(lexer->spaced.spaced_cmdline, &lexer->spaced_arr.sz);
	lexer->spaced_arr.spaced_cmdline_arr = (char **)minishell_calloc(
			lexer->spaced_arr.sz, sizeof(char *));
	if (!lexer->spaced_arr.spaced_cmdline_arr)
		return (STATUS_MALLOCERR);
	return (STATUS_SUCCESS);
}

t_status	lexer_cmd_split(t_lexer *lexer)
{
	t_status	status;
	uint32_t	count;
	char		*spaced;

	status = init_split(lexer);
	if (status)
		return (status);
	count = 0;
	spaced = lexer->spaced.spaced_cmdline;
	while (*spaced)
	{
		while (*spaced == SPACE)
			spaced++;
		if (!spaced)
			break ;
		status = cmd_split_word(&spaced, lexer, count);
		if (status)
			return (status);
		count++;
	}
	lexer->spaced_arr.spaced_cmdline_arr[count] = 0;
	return (STATUS_SUCCESS);
}

static t_status	lexer_cmd(t_lexer *lexer)
{
	t_status	status;

	if (lexer)
	{
		status = lexer_cmd_spaced(lexer);
		if (status)
			return (status);
		status = lexer_cmd_split(lexer);
		if (status)
			return (status);
		return (STATUS_SUCCESS);
	}
	return (STATUS_LEXERCMDERR);
} 

static t_status	lexer_init(t_lexer **lexer, t_minishell *ms)
{
	t_status	status;

	if (lexer)
	{
		(*lexer) = (t_lexer *)malloc(sizeof(t_lexer));
		if (!*lexer)
			return (STATUS_MALLOCERR);
		ms->lexer = *lexer;
		(*lexer)->token = NULL;
		(*lexer)->sztoken = 0;
		(*lexer)->cmdline = ms->cmdline;
		(*lexer)->spaced.spaced_cmdline = NULL;
		(*lexer)->spaced.sz = 0;
		status = lexer_cmd(*lexer);
		if (status)
			return (status);
		(*lexer)->current_position = 0;
		(*lexer)->sztoken = 0;
		(*lexer)->token = NULL;
		return (STATUS_SUCCESS);
	}
	return (STATUS_LINITERROR);
}

static	t_token_type	cmdsep_token_type(char *token_value)
{
	if (*token_value == CHAR_PIPE && *(token_value + 1) == CHAR_PIPE)
		return (TTOKEN_OR_OP);
	if (*token_value == CHAR_PIPE)
		return (TTOKEN_PIPE);
	if (*token_value == CHAR_AMPERSNAD && *(token_value + 1) == CHAR_AMPERSNAD)
		return (TTOKEN_AND_OP);
	if (*token_value == CHAR_GT || *token_value == CHAR_LT)
		return (TTOKEN_REDIRECT);
	if (*token_value == CHAR_PAREN_OPEN)
		return (TTOKEN_PARENTHESE_OPEN);
	if (*token_value == CHAR_PAREN_CLOSE)
		return (TTOKEN_PARENTHESE_CLOSE);
	return (TTOKEN_UNKOWN);
}

t_token_type	lex_get_token_type(char *token_value)
{
	if (minishell_iscmdsep(*token_value))
		return (cmdsep_token_type(token_value));
	else
	{
		if (*token_value == CHAR_DOLLAR_SIGN)
			return (TTOKEN_VARIABLE);
		if (*token_value == CHAR_DASH)
			return (TTOKEN_OPTION);
		if (*token_value == CHAR_SINGLE_QUOTE || *token_value == CHAR_DOUBLE_QUOTE)
			return (TTOKEN_STRING | TTOKEN_ARGUMENT);
		else
			return (TTOKEN_ARGUMENT);
	}
	return (TTOKEN_UNKOWN);
}

static t_token	*lex_last_token(t_token *token)
{
	if (token)
	{
		while (token->next_token)
			token = token->next_token;
		return (token);
	}
	return (NULL);
}

t_status	lex_add_token(t_lexer *lexer, char *tvalue, t_token_type ttype)
{
	t_token				*token;
	static	uint32_t	tid;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (STATUS_MALLOCERR);
	token->ttype = ttype;
	token->tid = tid++;
	token->tvalue = tvalue;
	token->next_token = NULL;
	if (!lexer->token)
		lexer->token = token;
	else
		lex_last_token(lexer->token)->next_token = token;
	return (STATUS_SUCCESS);
}

static t_status	lex_tflag(t_lexer *lexer, char *tvalue)
{
	t_token	*ltoken;

	ltoken = lex_last_token(lexer->token);
	if (ltoken && ltoken->ttype == TTOKEN_REDIRECT
		&& ltoken->tvalue[0] == CHAR_GT)
		return (lex_add_token(lexer, tvalue, TTOKEN_REDIRECT_FILE));
	else if (ltoken && ltoken->ttype == TTOKEN_REDIRECT
		&& ltoken->tvalue[0] == CHAR_LT)
		return (lex_add_token(lexer, tvalue, TTOKEN_REDIRECT_EOF));
	else
		return (lex_add_token(lexer, tvalue, TTOKEN_COMMAND));
}

static t_status	lex_lex(t_lexer *lexer, char **splited_cmd)
{
	bool		fflag;
	t_status	status;

	fflag = true;
	while (*splited_cmd)
	{
		if (minishell_iscmdsep(**splited_cmd))
		{
			status = lex_add_token(lexer, *splited_cmd,
				lex_get_token_type(*splited_cmd));
			fflag = true;
		}
		else if (fflag)
		{
			status = lex_tflag(lexer, *splited_cmd);
			fflag = false;
		}
		else
			status = lex_add_token(lexer, *splited_cmd,
				lex_get_token_type(*splited_cmd));
		if (status)
			return (status);
		splited_cmd++;
	}
	return (STATUS_SUCCESS);
}

t_status	lexer_lex(t_lexer *lexer)
{
	t_status	status;
	
	status = lex_lex(lexer, lexer->spaced_arr.spaced_cmdline_arr);
	if (status)
		return (status);
	return (STATUS_SUCCESS);
}


t_status	minishell_lexer(t_minishell *minishell)
{
	t_lexer		*lexer;
	t_status	status;

	status = lexer_init(&lexer, minishell);
	if (status)
		return (status);
	status = lexer_lex(lexer);
	if (status)
		return (status);
	return (STATUS_SUCCESS);
}

t_status	minishell_init(t_minishell **minishell)	
{
	if (minishell)
	{
		*minishell = (t_minishell *)malloc(sizeof(t_minishell));
		if (!minishell)
			return (STATUS_MALLOCERR);
		(*minishell)->prompt = "m0hc33n> ";
		return (STATUS_SUCCESS);
	}
	return (STATUS_MSINITERROR);
}

#define ERR(x) (printf("[!!] error number : %d\n", x))

void printtype(t_token_type ttype)
{
	if (ttype == TTOKEN_COMMAND)
		printf("COMMAND\n");
	else if (ttype == TTOKEN_OPTION)
		printf("OPTION\n");
	else if (ttype == TTOKEN_ARGUMENT)
		printf("ARGUMENT\n");
	else if (ttype == (TTOKEN_STRING | TTOKEN_ARGUMENT))
		printf("ARG STRING\n");
	else if (ttype == TTOKEN_PIPE)
		printf("PIPE\n");
	else if (ttype == TTOKEN_OR_OP)
		printf("OR_OP\n");
	else if (ttype == TTOKEN_AND_OP)
		printf("AND_OP\n");
	else if (ttype == TTOKEN_REDIRECT)
		printf("REDIRECTION\n");
	else if (ttype == TTOKEN_REDIRECT_FILE)
		printf("REDIRECT OUTPUT\n");
	else if (ttype == TTOKEN_REDIRECT_EOF)
		printf("REDIRECT INPUT 'EOF'\n");
	else if (ttype == TTOKEN_VARIABLE)
		printf("VARIABLE\n");
	else if (ttype == TTOKEN_PARENTHESE_OPEN)
		printf("OPEN PARENTHESE\n");
	else if (ttype == TTOKEN_PARENTHESE_CLOSE)
		printf("CLOSE PARENTHESE\n");
	else if (ttype == TTOKEN_UNKOWN)
		printf("UNKOWN\n");
}


int main(int ac, char **av)
{
	t_minishell	*minishell;
	t_status	status;

	status = minishell_init(&minishell);
	if (status) // TODO > minishell_init
		return (status); // TODO > minishell_error
	
	minishell->cmdline = readline(minishell->prompt);

	status = minishell_lexer(minishell);
	if (status)	 // WORKING ON IT
		return (status);
	// print tokens;
	t_token *token = minishell->lexer->token;
	while (token)
	{
		printf("===========================\n");
		printtype(token->ttype);
		printf("token id    : %d\n", token->tid);
		printf("token value : %s\n", token->tvalue);
		token = token->next_token;
	}

	return (0);
}
