#include "parser_internal.h"

static t_word	*new_word(t_arena *arena, char *str, int len, uint8_t flag)
{
	t_word	*word;

	word = ft_arena_calloc(arena, 1, sizeof(t_word));
	if (!word)
		return (NULL);
	word->str = str;
	word->len = len;
	word->flag = flag;
	return (word);
}

static t_word	*build_value(t_arena *arena, t_word *first, char *value_start)
{
	t_word	*head;
	int		rest;

	rest = first->len - (int)(value_start - first->str);
	if (rest < 0)
		rest = 0;
	head = new_word(arena, value_start, rest, W_NONE);
	if (!head)
		return (NULL);
	head->next = first->next;
	return (head);
}

t_assign	*word_to_assign(t_parser_state *ps, t_word *word)
{
	t_assign	*assign;
	int			key_len;

	if (!word || !word->eq_ptr)
		return (parser_fail(ps, ST_FATAL, NULL), NULL);
	assign = ft_arena_calloc(&ps->arenas->ast, 1, sizeof(t_assign));
	if (!assign)
		return (parser_fail(ps, ST_FATAL, NULL), NULL);
	key_len = (int)(word->eq_ptr - word->str);
	if (word->flag & W_APPEND)
		key_len--;
	assign->key = new_word(&ps->arenas->ast, word->str, key_len,
			word->flag & (W_ASSIGN | W_APPEND));
	assign->value = build_value(&ps->arenas->ast,
			word, word->eq_ptr + 1);
	if (!assign->key || !assign->value)
		return (parser_fail(ps, ST_FATAL, NULL), NULL);
	return (assign);
}
