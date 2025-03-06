#include "../../inc/tools.h"

char	*minishell_readfile(char *filename)
{
	int32_t	fd;
	int32_t	rbytes;
	char	buffer[FREAD_SIZE + 1];
	char	*data;
	char	*p;

	fd = open(fd, O_RDONLY);
	if (fd == -1)
		return (NULL);
	rbytes = FREAD_SIZE;
	data = NULL;
	while (rbytes >= FREAD_SIZE)
	{
		rbytes = read(fd, &buffer, FREAD_SIZE);
		buffer[FREAD_SIZE] = 0;
		p = data;
		data = minishell_strjoin(data, &buffer);
		if (p)
			free(p);
	}
	return (data);
}
