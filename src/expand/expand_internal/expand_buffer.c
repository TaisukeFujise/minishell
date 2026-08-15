#include "expand_internal.h"

bool	expand_buf_init(t_expand *exp)
{
	if (!strbuf_init(&exp->buf))
		return (false);
	if (strbuf_init(&exp->glob_mask))
		return (true);
	strbuf_free(&exp->buf);
	return (false);
}

void	expand_buf_free(t_expand *exp)
{
	strbuf_free(&exp->buf);
	strbuf_free(&exp->glob_mask);
}

void	expand_buf_reset(t_expand *exp)
{
	strbuf_reset(&exp->buf);
	strbuf_reset(&exp->glob_mask);
}

bool	expand_buf_append(t_expand *exp, const char *str, size_t len,
					bool glob)
{
	size_t	mark;

	mark = exp->buf.len;
	if (!strbuf_append(&exp->buf, str, len))
		return (false);
	if (!strbuf_append(&exp->glob_mask, str, len))
	{
		exp->buf.len = mark;
		exp->buf.data[mark] = '\0';
		return (false);
	}
	ft_memset(exp->glob_mask.data + mark, glob, len);
	return (true);
}
