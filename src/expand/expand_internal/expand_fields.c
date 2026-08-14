#include "expand_internal.h"

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
