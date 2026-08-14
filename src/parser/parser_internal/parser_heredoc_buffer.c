#include "parser_internal.h"

void	hd_buf_init(t_hd_buf *buf)
{
	buf->head = NULL;
	buf->tail = &buf->head;
	buf->len = 0;
}

bool	hd_buf_add(t_hd_buf *buf, t_arena *arena, char *line)
{
	t_hd_line	*node;
	size_t		line_len;

	line_len = ft_strlen(line);
	if (buf->len > SIZE_MAX - 2
		|| line_len > SIZE_MAX - buf->len - 2)
		return (false);
	node = ft_arena_alloc(arena, sizeof(t_hd_line));
	if (!node)
		return (false);
	node->str = line;
	node->len = line_len;
	node->next = NULL;
	*buf->tail = node;
	buf->tail = &node->next;
	buf->len += line_len + 1;
	return (true);
}

char	*hd_buf_join(t_hd_buf *buf, t_arena *arena)
{
	t_hd_line	*line;
	char		*content;
	char		*dst;

	content = ft_arena_alloc(arena, buf->len + 1);
	if (!content)
		return (NULL);
	dst = content;
	line = buf->head;
	while (line)
	{
		ft_memcpy(dst, line->str, line->len);
		dst += line->len;
		*dst++ = '\n';
		line = line->next;
	}
	*dst = '\0';
	return (content);
}
