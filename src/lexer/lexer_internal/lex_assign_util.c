/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_assign_util.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 12:40:40 by fendo             #+#    #+#             */
/*   Updated: 2026/08/15 03:08:57 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

static void	set_assign_info(t_word *head, size_t eq_pos, uint8_t flag)
{
	t_word	*part;

	head->eq_pos = eq_pos;
	part = head;
	while (part)
	{
		part->flag |= flag;
		part = part->next;
	}
}

void	apply_assign_info(t_word *head, const char *str)
{
	size_t	eq_pos;
	bool	append;

	if (!head)
		return ;
	eq_pos = str_assign_pos(str, &append);
	if (eq_pos > 0 && append)
		set_assign_info(head, eq_pos, W_APPEND);
	else if (eq_pos > 0)
		set_assign_info(head, eq_pos, W_ASSIGN);
}
