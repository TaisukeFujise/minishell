#include "expand_internal.h"

void	fields_init(t_fields *fields)
{
	fields->head = NULL;
	fields->tail = &fields->head;
	fields->pending_ws = false;
}

t_status	fields_emit(t_expand *exp, t_fields *fields)
{
	t_word_list	*node;

	if (fields->glob)
		node = expand_glob(exp);
	else
		node = field_insert(exp, fields->tail, exp->buf.text.data,
				exp->buf.text.len);
	if (!node)
		return (ST_FATAL);
	if (fields->glob)
		*fields->tail = node;
	while (*fields->tail)
		fields->tail = &(*fields->tail)->next;
	expand_buf_reset(&exp->buf);
	fields->glob = false;
	fields->pending_ws = false;
	fields->keep_empty = false;
	return (ST_OK);
}

t_word_list	*field_insert(t_expand *exp, t_word_list **link, const char *s,
					size_t len)
{
	t_word_list	*node;

	node = ft_arena_calloc(&exp->arenas->ast, 1, sizeof(t_word_list));
	if (!node)
		return (NULL);
	node->wd = ft_arena_calloc(&exp->arenas->ast, 1, sizeof(t_word));
	if (!node->wd)
		return (NULL);
	node->wd->str = ft_arena_strndup(&exp->arenas->ast, s, len);
	if (!node->wd->str)
		return (NULL);
	node->wd->len = len;
	node->next = *link;
	*link = node;
	return (node);
}
