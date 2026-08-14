#include "expand_internal.h"

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
	new_data = malloc(new_cap);
	if (!new_data)
		return (false);
	ft_memcpy(new_data, buf->data, buf->len + 1);
	free(buf->data);
	buf->data = new_data;
	buf->cap = new_cap;
	return (true);
}

bool	strbuf_init(t_strbuf *buf)
{
	buf->data = malloc(EXP_BUF_INIT);
	if (!buf->data)
		return (false);
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

bool	strbuf_add(t_strbuf *buf, const char *s, size_t len)
{
	if (len == 0)
		return (true);
	if (len > SIZE_MAX - buf->len - 1)
		return (false);
	if (!strbuf_reserve(buf, buf->len + len + 1))
		return (false);
	ft_memcpy(buf->data + buf->len, s, len);
	buf->len += len;
	buf->data[buf->len] = '\0';
	return (true);
}
