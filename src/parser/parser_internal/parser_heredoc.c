#include "parser_internal.h"

static bool	word_buf_size(t_word *word, size_t *size)
{
	*size = 1;
	while (word)
	{
		if (word->len > SIZE_MAX - *size)
			return (false);
		*size += word->len;
		word = word->next;
	}
	return (true);
}

static char	*word_join(t_word *word, t_arena *arena)
{
	char	*buf;
	size_t	size;
	size_t	offset;

	if (!arena || !word_buf_size(word, &size))
		return (NULL);
	buf = ft_arena_alloc(arena, size);
	if (!buf)
		return (NULL);
	offset = 0;
	while (word)
	{
		ft_memcpy(buf + offset, word->str, word->len);
		offset += word->len;
		word = word->next;
	}
	buf[offset] = '\0';
	return (buf);
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
	return (ft_arena_strndup(&ps->arenas->tmp, start,
			(size_t)(end - start)));
}

void	collect_one_heredoc(t_parser_state *ps, t_redirect *redir)
{
	char		*delim;
	char		*line;
	t_hd_buf	buf;

	ft_arena_reset(&ps->arenas->tmp);
	delim = word_join(&redir->target, &ps->arenas->tmp);
	if (!delim)
		return (parser_fail(ps, ST_FATAL, NULL));
	hd_buf_init(&buf);
	line = read_next_heredoc_line(ps);
	while (line && ft_strcmp(line, delim) != 0)
	{
		if (!hd_buf_add(&buf, &ps->arenas->tmp, line))
			return (parser_fail(ps, ST_FATAL, NULL));
		line = read_next_heredoc_line(ps);
	}
	if (!line)
		return (parser_fail(ps, ST_FAILURE, hd_eof_warn_msg(ps, delim)));
	redir->hd.raw_str.str = hd_buf_join(&buf, &ps->arenas->heredoc);
	if (!redir->hd.raw_str.str)
		return (parser_fail(ps, ST_FATAL, NULL));
	redir->hd.raw_str.len = buf.len;
}
