#include "expand_internal.h"

bool	expbuf_init(t_expbuf *buf, t_arena *arena)
{
	buf->data = ft_arena_alloc(arena, EXP_BUF_INIT);
	buf->smap = ft_arena_alloc(arena, EXP_BUF_INIT * sizeof(bool));
	buf->gmap = ft_arena_alloc(arena, EXP_BUF_INIT * sizeof(bool));
	if (!buf->data || !buf->smap || !buf->gmap)
		return (false);
	buf->len = 0;
	buf->cap = EXP_BUF_INIT;
	return (true);
}

static bool	expbuf_grow(t_expbuf *buf, int need, t_arena *arena)
{
	int		newcap;
	char	*newdata;
	bool	*newsmap;
	bool	*newgmap;

	if (buf->len + need <= buf->cap)
		return (true);
	newcap = buf->cap * 2;
	while (newcap < buf->len + need)
		newcap *= 2;
	newdata = ft_arena_realloc(arena, buf->data, buf->cap, newcap);
	newsmap = ft_arena_realloc(arena, buf->smap,
			buf->cap * sizeof(bool), newcap * sizeof(bool));
	newgmap = ft_arena_realloc(arena, buf->gmap,
			buf->cap * sizeof(bool), newcap * sizeof(bool));
	if (!newdata || !newsmap || !newgmap)
		return (false);
	buf->data = newdata;
	buf->smap = newsmap;
	buf->gmap = newgmap;
	buf->cap = newcap;
	return (true);
}

bool	expbuf_append(t_expbuf *buf, const char *s, int n,
			bool splittable, bool globbable, t_arena *arena)
{
	int	i;

	if (n <= 0)
		return (true);
	if (!expbuf_grow(buf, n, arena))
		return (false);
	i = 0;
	while (i < n)
	{
		buf->data[buf->len + i] = s[i];
		buf->smap[buf->len + i] = splittable;
		buf->gmap[buf->len + i] = globbable;
		i++;
	}
	buf->len += n;
	return (true);
}

char	*expbuf_finish(t_expbuf *buf, t_arena *arena)
{
	char	*result;

	result = ft_arena_strndup(arena, buf->data, buf->len);
	return (result);
}
