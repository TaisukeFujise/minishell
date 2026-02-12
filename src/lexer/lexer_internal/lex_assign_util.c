/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_assign_util.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 12:40:40 by fendo             #+#    #+#             */
/*   Updated: 2026/02/11 22:41:18 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

void	set_assign_info(t_assign_info *as, t_assign_state state,
	char *eq_ptr, uint8_t flag)
{
	as->state = state;
	as->eq_ptr = eq_ptr;
	as->flag = flag;
}

void	validate_assign(char *cur_ptr, t_assign_info *as)
{
	if (as->state == AS_INIT)
	{
		as->state = AS_INVALID;
		if (ft_isalpha(*cur_ptr) || *cur_ptr == '_')
			as->state = AS_VALID;
	}
	else if (as->state == AS_VALID)
	{
		if (ft_strncmp(cur_ptr, "+=", 2) == 0)
			set_assign_info(as, AS_DONE, cur_ptr + 1, as->flag | W_APPEND);
		else if (*cur_ptr == '=')
			set_assign_info(as, AS_DONE, cur_ptr, as->flag | W_ASSIGN);
		else if (!(ft_isalnum(*cur_ptr) || *cur_ptr == '_'))
			as->state = AS_INVALID;
	}
}

void	apply_assign_info(t_word *head, t_assign_info *as)
{
	t_word	*cur;

	if (!head)
		return ;
	if (as->state == AS_VALID && as->flag == W_NONE)
		as->flag |= W_IDENT;
	head->eq_ptr = as->eq_ptr;
	head->flag |= as->flag;
	if (!(as->flag & (W_ASSIGN | W_APPEND)))
		return ;
	cur = head;
	while (cur)
	{
		cur->flag |= (as->flag & (W_ASSIGN | W_APPEND));
		cur = cur->next;
	}
}
