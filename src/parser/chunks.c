#include "../../inc/parser.h"

static t_status	add_chunk(t_chunk **chunks, char *content, bool interpret, t_env *env);
static void		link_chunk(t_chunk **chunks, t_chunk *chunk);

t_chunk		*minishell_chunker(char *value, t_env *env) // split for norminette WSL
{
	char		quote = 0;
	bool		interpret = false;
	uint32_t	s = 0;
	uint32_t	e = 0;
	char			*content;
	t_chunk	*chunks = NULL;

	while (value[s])
	{
		if (!quote && (value[e] == CHAR_SINGLE_QUOTE || value[e] == CHAR_DOUBLE_QUOTE))
		{
			quote = value[e++];
			s += 1;
		}
		while (value[e])
		{
			if (!quote && (value[e] == CHAR_SINGLE_QUOTE || value[e] == CHAR_DOUBLE_QUOTE || !value[e + 1]))
			{
				interpret = true;
				quote = value[e];
				e += (!value[e + 1]);
				break ;
			}
			if (quote && value[e] == quote)
			{
				interpret = (quote == CHAR_DOUBLE_QUOTE);
				quote = 0;
				break ;
			}
			e += 1;
		}
		content = (char *)malloc(sizeof(char) * (e - s + 1));
		if (!content)
			return (free_chunks(chunks), NULL);
		minishell_strlcpy(content, value + s, e - s + 1);
		if (add_chunk(&chunks, content, interpret, env))
			return (free(content), free_chunks(chunks), NULL);
		if (!value[e])
			break ;
		e += 1;
		s = e;
	}
	return (chunks);
}

static t_status	add_chunk(t_chunk **chunks, char *content, bool interpret, t_env *env)
{
	t_chunk	*chunk;
	char	*temp;

	chunk = (t_chunk *)malloc(sizeof(t_chunk));
	if (!chunk)
		return (STATUS_MALLOCERR);
	if (interpret && minishell_strchr(content, CHAR_DOLLAR_SIGN))
	{
		temp = minishell_expand(content, env);
		if (!temp)
			return (free(chunk), STATUS_MALLOCERR);
		free(content);
		content = temp;
	}
	chunk->content = content;
	chunk->next = NULL;
	link_chunk(chunks, chunk);
	return (STATUS_SUCCESS);
}

static void		link_chunk(t_chunk **chunks, t_chunk *chunk)
{
	t_chunk	*last;

	if (!*chunks)
		*chunks = chunk;
	else
	{
		last = *chunks;
		while (last->next)
			last = last->next;
		last->next = chunk;
	}
}

void	free_chunks(t_chunk *chunks)
{
	t_chunk	*next;

	while (chunks)
	{
		next = chunks->next;
		free(chunks->content);
		free(chunks);
		chunks = next;
	}
}
