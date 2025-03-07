#include "../../inc/tools.h"

bool		minishell_isred(t_root *node)
{
	if (!node)
		return(false);
	return (
		node->ttype == TTOKEN_HEREDOC ||
		node->ttype == TTOKEN_OUTPUT ||
		node->ttype == TTOKEN_APPEND ||
		node->ttype == TTOKEN_INPUT
	);
}

void	generate_filename(int32_t fd, char *filename)
{
	int32_t	rbytes;
	int32_t	i;

	rbytes = read(fd, filename, minishell_strlen(TMP) - 1);
    if (rbytes != (int32_t)minishell_strlen(TMP) - 1)
	{
        perror("Error reading random bytes");
        free(filename);
        close(fd);
        return ;
    }
	i = 0;
    while (TMP[i] != '0' && TMP[i] != '.')
	{
		filename[i] = TMP[i];
		i++;
    }
    filename[strlen(TMP) - 1] = '\0';
    close(fd);
}

/* 
 * generate a random file name
 * path : /tmp
*/
char	*minishell_generate_filename()
{
	int32_t	fd;
	char	*filename;

	fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) 
	{
        perror("Error opening /dev/urandom");
        return (NULL);
    }
    filename = (char *)minishell_calloc(minishell_strlen(TMP) + 1, 1);
    if (filename == NULL) 
	{
        perror("Memory allocation failed");
        close(fd);
        return (NULL);
    }
    generate_filename(fd, filename);
	return (filename);
}

void	hdoc_sigint(int32_t signum)
{
	if (signum == SIGINT)
		exit(STATUS_HDOCSIGINT);
}
