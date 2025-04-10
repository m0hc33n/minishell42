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
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	if (g_sig_pid == 0)
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
