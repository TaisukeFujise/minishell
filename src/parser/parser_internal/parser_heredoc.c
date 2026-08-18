#include "parser_internal.h"
#include "strbuf.h"

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
		start = shell_read_line("> ");
		if (!start || g_signum == SIGINT)
			return (free(start), NULL);
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

static bool	read_heredoc_body(t_parser_state *ps, char *delim, t_strbuf *buf)
{
	char	*line;

	line = read_next_heredoc_line(ps);
	while (line && ft_strcmp(line, delim) != 0)
	{
		if (!strbuf_append(buf, line, ft_strlen(line))
			|| !strbuf_append(buf, "\n", 1))
		{
			parser_fail(ps, ST_FATAL, NULL);
			return (false);
		}
		line = read_next_heredoc_line(ps);
	}
	if (line)
		return (true);
	if (g_signum == SIGINT)
	{
		ps->status = ST_FAILURE;
		ps->ctx->err.exit_code = 130;
		return (false);
	}
	parser_fail(ps, ST_FAILURE, hd_eof_warn_msg(ps, delim));
	return (false);
}

void	collect_one_heredoc(t_parser_state *ps, t_redirect *redir)
{
	char		*delim;
	t_strbuf	buf;

	ft_arena_reset(&ps->arenas->tmp);
	delim = word_join(&redir->target, &ps->arenas->tmp);
	if (!delim)
		return (parser_fail(ps, ST_FATAL, NULL));
	if (!strbuf_init(&buf))
		return (parser_fail(ps, ST_FATAL, NULL));
	if (!read_heredoc_body(ps, delim, &buf))
	{
		strbuf_free(&buf);
		return ;
	}
	redir->hd.raw_str.str = strbuf_detach(&buf, &redir->hd.raw_str.len);
}
