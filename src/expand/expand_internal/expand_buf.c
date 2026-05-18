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
	size_t	new_cap;

	if (len == 0)
		return (true);
	if (buf->len + len + 1 > buf->cap)
	{
		new_cap = buf->cap * 2;
		while (new_cap < buf->len + len + 1)
			new_cap *= 2;
		new_data = ft_arena_realloc(buf->arena, buf->data,
				buf->cap, new_cap);
		if (!new_data)
			return (false);
		buf->data = new_data;
		buf->cap = new_cap;
	}
	ft_memcpy(buf->data + buf->len, s, len);
	buf->len += len;
	buf->data[buf->len] = '\0';
	return (true);
}

bool	fields_init(t_fields *fields, t_arena *arena)
{
	if (!strbuf_init(&fields->buf, arena))
		return (false);
	fields->head = NULL;
	fields->tail = &fields->head;
	fields->glob = false;
	fields->emitted = false;
	fields->keep_empty = false;
	return (true);
}

t_status	fields_emit(t_expand *ex, t_fields *fields)
{
	t_word_list	*node;

	if (fields->glob)
		node = append_glob(ex, fields, fields->buf.data);
	else
		node = fields_add(ex, fields, fields->buf.data, fields->buf.len);
	if (!node)
		return (ST_FATAL);
	fields->buf.len = 0;
	fields->buf.data[0] = '\0';
	fields->glob = false;
	fields->emitted = true;
	return (ST_OK);
}

t_word_list	*fields_add(t_expand *ex, t_fields *fields, const char *s,
				size_t len)
{
	t_word_list	*node;

	node = ft_arena_calloc(&ex->arenas->ast, 1, sizeof(t_word_list));
	if (!node)
		return (NULL);
	node->wd = ft_arena_calloc(&ex->arenas->ast, 1, sizeof(t_word));
	if (!node->wd)
		return (NULL);
	node->wd->str = ft_arena_strndup(&ex->arenas->ast, s, len);
	if (!node->wd->str)
		return (NULL);
	node->wd->len = (int)len;
	*fields->tail = node;
	fields->tail = &node->next;
	return (node);
}
