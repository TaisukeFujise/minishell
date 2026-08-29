/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_assign.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 19:36:10 by fendo             #+#    #+#             */
/*   Updated: 2026/08/25 19:36:11 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

static t_word	*new_word(t_arena *arena, char *str, size_t len, uint8_t flag)
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

static t_word	*build_value(t_arena *arena, t_word *first)
{
	t_word	*head;

	head = new_word(arena, first->str + first->eq_pos + 1,
			first->len - first->eq_pos - 1, W_NONE);
	if (!head)
		return (NULL);
	head->next = first->next;
	return (head);
}

t_assign	*word_to_assign(t_parser_state *ps, t_word *word)
{
	t_assign	*assign;
	size_t		key_len;

	assign = ft_arena_calloc(&ps->arenas->ast, 1, sizeof(t_assign));
	if (!assign)
		return (parser_fail(ps, ST_FATAL, NULL), NULL);
	key_len = word->eq_pos;
	if (word->flag & W_APPEND)
		key_len--;
	assign->key = new_word(&ps->arenas->ast, word->str, key_len,
			word->flag & (W_ASSIGN | W_APPEND));
	assign->value = build_value(&ps->arenas->ast, word);
	if (!assign->key || !assign->value)
		return (parser_fail(ps, ST_FATAL, NULL), NULL);
	return (assign);
}
