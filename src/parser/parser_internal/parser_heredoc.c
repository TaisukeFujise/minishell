#include "parser_internal.h"

static char	*append_line(t_arena *arena, char *content, size_t *len, char *line)
{
	char	*new_content;
	size_t	line_len;

	line_len = ft_strlen(line);
	if (line_len > SIZE_MAX - *len - 2)
		return (NULL);
	new_content = ft_arena_realloc(arena, content, *len + 1,
			*len + line_len + 2);
	if (!new_content)
		return (NULL);
	ft_memcpy(new_content + *len, line, line_len);
	*len += line_len;
	new_content[(*len)++] = '\n';
	new_content[*len] = '\0';
	return (new_content);
}

static size_t	delim_len(t_word *word, bool *quoted)
{
	size_t	len;

	len = 0;
	*quoted = false;
	while (word)
	{
		if ((word->flag & (W_SQ | W_DQ)) != 0)
			*quoted = true;
		len += (size_t)word->len;
		word = word->next;
	}
	return (len);
}

static bool	join_delim(t_word *word, t_arena *arena)
{
	char	*buf;
	char	*p;
	bool	quoted;
	size_t	len;
	t_word	*cur;

	len = delim_len(word, &quoted);
	buf = ft_arena_alloc(arena, len + 1);
	if (!buf)
		return (false);
	p = buf;
	cur = word;
	while (cur)
	{
		p = (char *)ft_memcpy(p, cur->str, cur->len) + cur->len;
		cur = cur->next;
	}
	*p = '\0';
	word->str = buf;
	word->len = (int)len;
	word->flag = W_SQ * quoted;
	word->next = NULL;
	return (true);
}

static char	*read_next_heredoc_line(t_parser_state *ps)
{
	char	*start;
	char	*end;
	char	*line;

	if (*(ps->lex.line) == '\0')
	{
		start = readline("> ");
		if (!start)
			return (NULL);
		line = ft_arena_strdup(&ps->arenas->tmp, start);
		free(start);
		return (line);
	}
	start = ps->lex.line;
	end = start;
	while (*end && *end != '\n')
		end++;
	ps->lex.line = end;
	if (*end == '\n')
		ps->lex.line++;
	return (ft_arena_strndup(&ps->arenas->tmp, start, end - start));
}

void	collect_one_heredoc(t_parser_state *ps, t_redirect *redir)
{
	char	*line;
	char	*content;
	size_t	len;

	ft_arena_reset(&ps->arenas->tmp);
	if (!join_delim(&redir->target, &ps->arenas->ast))
		return (parser_fail(ps, ST_FATAL, NULL));
	content = ft_arena_strdup(&ps->arenas->heredoc, "");
	if (!content)
		return (parser_fail(ps, ST_FATAL, NULL));
	len = 0;
	line = read_next_heredoc_line(ps);
	while (line && ft_strcmp(line, redir->target.str) != 0)
	{
		content = append_line(&ps->arenas->heredoc, content, &len, line);
		if (!content)
			return (parser_fail(ps, ST_FATAL, NULL));
		line = read_next_heredoc_line(ps);
	}
	if (!line)
		ft_putendl_fd(hd_eof_warn_msg(ps, redir->target.str), STDERR_FILENO);
	redir->hd.raw_str.str = content;
	redir->hd.raw_str.len = len;
}
