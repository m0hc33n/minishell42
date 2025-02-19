#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <wait.h>
# include "lexer.h"
# include "parser.h"s

// SHELL PROMPT
# define PROMPT "MINISHELL> "

/* MINISHELL STATUS
 * INITFAIL : cant initialise s_minishell
 * SYNTERR  : cmdline syntax erro
 * LEXERCMDERR : lexer_cmd bad argument
*/
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
	STATUS_EMPTYCMD			= 0x000008
}	t_status;

typedef struct s_minishell
{
	char	*prompt;
	char	*cmdline;
	t_lexer	*lexer;
	t_root	*root;
	
}	t_minishell;

// INIT
t_status	minishell_init(t_minishell **minishell);

#endif