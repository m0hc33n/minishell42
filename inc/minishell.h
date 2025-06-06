#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <wait.h>
# include <sys/types.h>
# include <signal.h>
# include <dirent.h>
# include <termios.h>
# include "structs.h"
# include "lexer.h"
# include "parser.h"
# include "executor.h"
# include "tools.h"
# include "builtins.h"

// SHELL PROMPT
# define PROMPT "MINISHELL> "

// INIT
t_status	minishell_init(t_minishell **minishell, char **env);

// SIGNALS
t_status	siginit_init(int32_t signum, void (*sighandler)(int32_t));

#endif