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

	rbytes = read(fd, filename, FNAME_SIZE);
    if (rbytes != FNAME_SIZE)
	{
        perror("Error reading random bytes");
        free(filename);
        close(fd);
        return ;
    }
	minishell_strlcpy(filename, "/tmp/.", 7);
	i = 6;
    while (i < FNAME_SIZE)
	{
		filename[i] = "0123456789ABCDEF"[filename[i] % 16];
		i++;
    }
    filename[i] = 0; 
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
    filename = (char *)minishell_calloc(1, FNAME_SIZE + 1);
    if (filename == NULL) 
	{
        perror("Memory allocation failed");
        close(fd);
        return (NULL);
    }
    generate_filename(fd, filename);
	printf("filename[%s]", filename);
	return (filename);
}

void	hdoc_sigint(int32_t signum)
{
	if (signum == SIGINT)
		exit(STATUS_HDOCSIGINT);
}
