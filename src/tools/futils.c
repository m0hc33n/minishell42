#include "../../inc/tools.h"

char		*readfile_join(const char *s1, const char *s2)
{
	size_t	total_len;
	char	*res;

	if (!s1)
		return (minishell_strdup(s2));
	if (!s2)
		return (minishell_strdup(s1));
	total_len = minishell_strlen(s1) + minishell_strlen(s2);
	res = (char *)malloc(sizeof(char) * (total_len + 1));
	if (res == NULL)
		return (NULL);
	minishell_strlcpy(res, s1, minishell_strlen(s1) + 1);
	minishell_strlcat(res, s2, total_len + 1);
	return (res);
}

char	*minishell_readfile(char *filename)
{
	int32_t	fd;
	int32_t	rbytes;
	char	buffer[FREAD_SIZE + 1];
	char	*data;
	char	*p;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (NULL);
	rbytes = FREAD_SIZE;
	data = NULL;
	while (rbytes >= FREAD_SIZE)
	{
		rbytes = read(fd, &buffer, FREAD_SIZE);
		buffer[rbytes] = 0;
		p = data;
		data = readfile_join(data, buffer);
		if (p)
		 	free(p);
	}
	return (data);
}
