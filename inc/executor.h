#ifndef EXECUTOR_H
# define EXECUTOR_H

#include "minishell.h"

# define CHILD_PROCESS 0x000000

// EXECUTION
t_status	minishell_executor(t_minishell *minishell);
void		exec_redirect(t_root *root, int32_t *exit_code);
void		exec_pipe(t_root *root, int32_t *exit_code);
void		exec_cmd(t_root *root, int32_t *exit_code);

// UTILS
char		**executor_getargs(t_root *root);

#endif