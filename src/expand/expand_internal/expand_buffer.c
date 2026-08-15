#include "expand_internal.h"

/*
	One allocation holds both halves: data occupies [0, cap) and mask
	[cap, 2 * cap). Growing therefore moves them together and free(data)
	still releases everything.
*/
static bool	strbuf_reserve(t_strbuf *buf, size_t required)
{
	char	*new_data;
	size_t	new_cap;

	if (required <= buf->cap)
		return (true);
	new_cap = required;
	if (buf->cap <= SIZE_MAX / 2)
	{
		new_cap = buf->cap * 2;
		if (new_cap < required)
			new_cap = required;
	}
	if (new_cap > SIZE_MAX / 2)
		return (false);
	new_data = malloc(new_cap * 2);
	if (!new_data)
		return (false);
	ft_memcpy(new_data, buf->data, buf->len + 1);
	ft_memcpy(new_data + new_cap, buf->mask, buf->len);
	free(buf->data);
	buf->data = new_data;
	buf->mask = new_data + new_cap;
	buf->cap = new_cap;
	return (true);
}

bool	strbuf_init(t_strbuf *buf)
{
	buf->data = malloc(EXP_BUF_INIT * 2);
	if (!buf->data)
		return (false);
	buf->mask = buf->data + EXP_BUF_INIT;
	buf->data[0] = '\0';
	buf->len = 0;
	buf->cap = EXP_BUF_INIT;
	return (true);
}

void	strbuf_reset(t_strbuf *buf)
{
	buf->len = 0;
	buf->data[0] = '\0';
}

bool	strbuf_add(t_strbuf *buf, const char *s, size_t len, bool glob)
{
	if (len == 0)
		return (true);
	if (len > SIZE_MAX - buf->len - 1)
		return (false);
	if (!strbuf_reserve(buf, buf->len + len + 1))
		return (false);
	ft_memcpy(buf->data + buf->len, s, len);
	ft_memset(buf->mask + buf->len, glob, len);
	buf->len += len;
	buf->data[buf->len] = '\0';
	return (true);
}
