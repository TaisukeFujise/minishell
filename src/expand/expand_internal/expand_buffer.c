#include "expand_internal.h"

bool	strbuf_init(t_strbuf *buf, t_arena *arena)
{
	buf->arena = arena;
	buf->data = ft_arena_alloc(arena, EXP_BUF_INIT);
	if (!buf->data)
		return (false);
	buf->len = 0;
	buf->cap = EXP_BUF_INIT;
	buf->data[0] = '\0';
	return (true);
}

bool	strbuf_add(t_strbuf *buf, const char *s, size_t len)
{
	char	*new_data;
	size_t	required;

	if (len == 0)
		return (true);
	if (len > SIZE_MAX - buf->len - 1)
		return (false);
	required = buf->len + len + 1;
	if (required > buf->cap)
	{
		new_data = ft_arena_realloc(buf->arena, buf->data,
				buf->cap, required);
		if (!new_data)
			return (false);
		buf->data = new_data;
		buf->cap = required;
	}
	ft_memcpy(buf->data + buf->len, s, len);
	buf->len += len;
	buf->data[buf->len] = '\0';
	return (true);
}
