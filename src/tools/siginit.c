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
	g_sig.is_sig = true;
	if (g_sig.is_hdoc)
	{
		g_sig.is_hdoc = 0;
		exit(STATUS_HDOCSIGINT);
	}
	else
		write(STDERR_FILENO, "\nMINISHELL> ", 12);
}

// void	sigquit_handler(int32_t signum)
// {
// 	(void)signum;
// }

t_status	minishell_siginit(void)
{
	t_status	status;

	status = siginit_init(SIGINT, sigint_handler);
	if (status)
		return (status);
	signal(SIGQUIT, SIG_IGN);
	return (STATUS_SUCCESS);
}
