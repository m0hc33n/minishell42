#include "../../inc/parser.h"

static int	s_count(char const *pattern, bool *asterisk);
static int	ft_split(char const *pattern, bool *asterisk, char **split, int count);

char	**asterisk_split(char *pattern, bool *asterisk)
{
	char	**split;
	int		count;

	if (!pattern)
		return (NULL);
	count = s_count(pattern, asterisk);
	split = (char **)malloc(sizeof(char *) * (count + 1));
	if (split == NULL)
		return (NULL);
	if (!ft_split(pattern, asterisk, split, count))
		return (NULL);
	return (split);
}

static int	s_count(char const *pattern, bool *asterisk)
{
	int		i;
	int		a;
	int		count;

	i = 0;
	a = 0;
	count = 0;
	while (pattern[i])
	{
		if (pattern[i] == '*' && asterisk[a])
		{
			i++;
			a++;
		}
		else
		{
			count++;
			while (pattern[i] && (pattern[i] != '*' || !asterisk[a]))
			{
				a += (pattern[i] == '*');
				i++;
			}
		}
	}
	return (count);
}

static int	ft_split(char const *pattern, bool *asterisk, char **split, int count)
{
	int	i;
	int	a;
	int	st;
	int	split_i;

	i = 0;
	a = 0;
	split_i = 0;
	while (split_i < count)
	{
		while (pattern[i] && pattern[i] == '*' && asterisk[a])
		{
			a++;
			i++;
		}
		st = i;
		while (pattern[i] && (pattern[i] != '*' || !asterisk[a]))
		{
			a += (pattern[i] == '*');
			i++;
		}
		split[split_i] = (char *)malloc(sizeof(char) * (i - st + 1));
		if (split[split_i] == NULL)
		{
			minishell_free_arr(split);
			return (0);
		}
		minishell_strlcpy(split[split_i], pattern + st, i - st + 1);
		split_i++;
	}
	split[count] = NULL;
	return (1);
}
