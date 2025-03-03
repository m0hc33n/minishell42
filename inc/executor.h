#ifndef EXECUTOR_H
# define EXECUTOR_H

#include "minishell.h"

# define HRD_BUFFER_SIZE 1024

# define CHILD_PROCESS 0x00000000
# define PIPE_READ_END 0x00000000
# define PIPE_WRITE_END 0x00000001

# define TMP "/tmp"

// EXECUTION
t_status	minishell_executor(t_minishell *minishell);
void		exec_redirect(t_minishell *minishell, t_root *node,
				int32_t input_fd, int32_t output_fd);
void		exec_pipe(t_minishell *minishell, t_root *root);
void		exec_cmd(t_minishell *minishell, t_root *root, int32_t output_fd);

// UTILS
char		**executor_getargs(t_root *root);
void		redirect_output(t_root *node, int32_t output_fd);
void		redirect_append(t_root *node, int32_t output_fd);
void		redirect_input(t_root *node, int32_t input_fd);

#endif