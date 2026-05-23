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

t_status	fields_emit(t_expand *exp, t_fields *fields)
{
	t_word_list	*node;

	if (fields->glob)
		node = expand_glob(exp, fields->buf.data);
	else
		node = field_insert(exp, fields->tail, fields->buf.data,
				fields->buf.len);
	if (!node)
		return (ST_FATAL);
	if (fields->glob)
		*fields->tail = node;
	while (*fields->tail)
		fields->tail = &(*fields->tail)->next;
	fields->buf.len = 0;
	fields->buf.data[0] = '\0';
	fields->glob = false;
	fields->emitted = true;
	return (ST_OK);
}

t_word_list	*field_insert(t_expand *exp, t_word_list **link, const char *s,
					size_t len)
{
	t_word_list	*node;

	if (len > INT_MAX)
		return (NULL);
	node = ft_arena_calloc(&exp->arenas->ast, 1, sizeof(t_word_list));
	if (!node)
		return (NULL);
	node->wd = ft_arena_calloc(&exp->arenas->ast, 1, sizeof(t_word));
	if (!node->wd)
		return (NULL);
	node->wd->str = ft_arena_strndup(&exp->arenas->ast, s, len);
	if (!node->wd->str)
		return (NULL);
	node->wd->len = (int)len;
	node->next = *link;
	*link = node;
	return (node);
}
