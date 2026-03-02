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

static char	*word_join(t_word *word, t_arena *arena)
{
	char	*buf;
	size_t	len;

	if (!arena)
		return (NULL);
	buf = ft_arena_alloc(arena, 1);
	len = 0;
	while (buf && word)
	{
		buf = ft_arena_realloc(arena, buf, len + 1,
				len + word->len + 1);
		if (buf)
			ft_memcpy(buf + len, word->str, word->len);
		len += (size_t)word->len;
		word = word->next;
	}
	if (buf)
		buf[len] = '\0';
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
	return (ft_arena_strndup(&ps->arenas->tmp, start, end - start));
}

void	collect_one_heredoc(t_parser_state *ps, t_redirect *redir)
{
	char	*delim;
	char	*line;
	char	*content;
	size_t	len;

	ft_arena_reset(&ps->arenas->tmp);
	delim = word_join(&redir->target, &ps->arenas->tmp);
	content = ft_arena_strdup(&ps->arenas->heredoc, "");
	if (!delim || !content)
		return (parser_fail(ps, ST_FATAL, NULL));
	len = 0;
	line = read_next_heredoc_line(ps);
	while (line && ft_strcmp(line, delim) != 0)
	{
		content = append_line(&ps->arenas->heredoc, content, &len, line);
		if (!content)
			return (parser_fail(ps, ST_FATAL, NULL));
		line = read_next_heredoc_line(ps);
	}
	if (!line)
		return (parser_fail(ps, ST_FAILURE, hd_eof_warn_msg(ps, delim)));
	redir->hd.raw_str.str = content;
	redir->hd.raw_str.len = len;
}
