/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_word_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 22:00:38 by fendo             #+#    #+#             */
/*   Updated: 2026/02/04 14:47:57 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"
#include "minishell.h"

int	is_tk_bound(char *ch)
{
	return (ft_strchr(" \t\n|()><", *ch) || strchunk("&&||>><<", ch, 2));
}

int	append_part(t_word ***tail, char *str, int len, uint8_t flag)
{
	t_word	*part;

	if ((len < 0) || (len == 0 && !(flag & (W_SQ | W_DQ))))
		return (0);
	part = ft_calloc(1, sizeof(t_word));
	if (!part)
		return (-1);
	part->str = str;
	part->len = len;
	part->flag = flag;
	**tail = part;
	*tail = &part->next;
	return (0);
}

int	finish_quote(char **line, t_word ***tail, char *begin, uint8_t flag)
{
	const char	*quote;

	quote = "\"";
	if (flag & W_SQ)
		quote = "\'";
	if (ft_strncmp(*line, quote, 1) != 0)
		return (ERR_UNCLOSED_QUOTE);
	if (append_part(tail, begin, *line - begin, flag) < 0)
		return (-1);
	(*line)++;
	return (0);
}

void	lex_dollar(char **cur_ptr, uint8_t *flag)
{
	(*cur_ptr)++;
	if (!ft_strncmp(*cur_ptr, "?", 1))
	{
		(*cur_ptr)++;
		*flag = W_DOLL;
		return ;
	}
	else if (ft_isalpha(**cur_ptr) || !ft_strncmp(*cur_ptr, "_", 1))
	{
		(*cur_ptr)++;
		while (ft_isalnum(**cur_ptr) || !ft_strncmp(*cur_ptr, "_", 1))
			(*cur_ptr)++;
		*flag = W_DOLL;
	}
}

void	validate_assign(char *cur_ptr, t_assign_info *as)
{
	if (as->state == AS_DONE || as->state == AS_INVALID)
		return ;
	if (!ft_strncmp(cur_ptr, "=", 1))
	{
		if (as->state == AS_VALID)
		{
			as->eq_ptr = cur_ptr;
			as->flag |= W_ASSIGN;
		}
		as->state = AS_DONE;
		return ;
	}
	if (as->state == AS_INIT)
	{
		if (ft_isalpha(*cur_ptr) || !ft_strncmp(cur_ptr, "_", 1))
			as->state = AS_VALID;
		else
			as->state = AS_INVALID;
	}
	else
	{
		if (!(ft_isalnum(*cur_ptr) || !ft_strncmp(cur_ptr, "_", 1)))
			as->state = AS_INVALID;
	}
}
