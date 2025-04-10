#include "../../inc/tools.h"

t_status	siginit_init(int32_t signum, void (*sighandler)(int32_t))
{
	struct sigaction	sa;

	sa.sa_handler = sighandler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (-1 == sigaction(signum, &sa, NULL))
		return (STATUS_SIGINIT);
	return (STATUS_SUCCESS);
}

void	sigint_handler(int32_t signum)
{
	(void)signum;
	if (g_sig.is_hdoc)
	{
		g_sig.is_hdoc = 0;
		g_sig.is_sig = false;
		exit(STATUS_HDOCSIGINT);
	}
	else
		write(STDOUT_FILENO, "\n", 1);
	g_sig.is_sig = true;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

t_status	minishell_siginit(void)
{
	t_status	status;

	status = siginit_init(SIGINT, sigint_handler);
	if (status)
		return (status);
	signal(SIGQUIT, SIG_IGN);
	return (STATUS_SUCCESS);
}
