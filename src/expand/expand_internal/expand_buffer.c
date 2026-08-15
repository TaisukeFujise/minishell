#include "expand_internal.h"

bool	expand_buf_init(t_expand_buf *buf)
{
	if (!strbuf_init(&buf->text))
		return (false);
	if (strbuf_init(&buf->mask))
		return (true);
	strbuf_free(&buf->text);
	return (false);
}

void	expand_buf_free(t_expand_buf *buf)
{
	strbuf_free(&buf->text);
	strbuf_free(&buf->mask);
}

void	expand_buf_reset(t_expand_buf *buf)
{
	strbuf_reset(&buf->text);
	strbuf_reset(&buf->mask);
}

bool	expand_buf_append(t_expand_buf *buf, const char *str, size_t len,
					bool glob)
{
	size_t	mark;

	mark = buf->text.len;
	if (!strbuf_append(&buf->text, str, len))
		return (false);
	if (!strbuf_append(&buf->mask, str, len))
	{
		buf->text.len = mark;
		buf->text.data[mark] = '\0';
		return (false);
	}
	ft_memset(buf->mask.data + mark, glob, len);
	return (true);
}
