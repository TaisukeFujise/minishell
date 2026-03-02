#include "expand_internal.h"

t_word_list	*make_word_node(t_expand_ctx *ex, const char *s)
{
	t_word_list	*node;
	t_word		*wd;

	node = ft_arena_calloc(ex->ast, 1, sizeof(t_word_list));
	wd = ft_arena_calloc(ex->ast, 1, sizeof(t_word));
	if (!node || !wd)
		return (NULL);
	wd->str = ft_arena_strdup(ex->ast, s);
	if (!wd->str)
		return (NULL);
	wd->len = ft_strlen(s);
	wd->flag = W_NONE;
	node->wd = wd;
	return (node);
}

t_word_list	*fields_to_wordlist(t_expand_ctx *ex, char **fields, int count)
{
	t_word_list	*head;
	t_word_list	*tail;
	t_word_list	*node;
	int			i;

	head = NULL;
	tail = NULL;
	i = 0;
	while (i < count)
	{
		node = make_word_node(ex, fields[i]);
		if (!node)
			return (NULL);
		if (!head)
			head = node;
		else
			tail->next = node;
		tail = node;
		i++;
	}
	return (head);
}

bool	word_has_quote(t_word *wd)
{
	while (wd)
	{
		if (wd->flag & (W_SQ | W_DQ))
			return (true);
		wd = wd->next;
	}
	return (false);
}

char	*word_to_str(t_expand_ctx *ex, t_word *wd)
{
	int		total;
	char	*result;
	char	*p;
	t_word	*cur;

	total = 0;
	cur = wd;
	while (cur)
	{
		total += cur->len;
		cur = cur->next;
	}
	result = ft_arena_alloc(ex->ast, total + 1);
	if (!result)
		return (NULL);
	p = result;
	while (wd)
	{
		ft_memcpy(p, wd->str, wd->len);
		p += wd->len;
		wd = wd->next;
	}
	*p = '\0';
	return (result);
}
