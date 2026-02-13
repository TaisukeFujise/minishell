/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_word_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 22:00:38 by fendo             #+#    #+#             */
/*   Updated: 2026/02/11 21:55:44 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"
#include "minishell.h"

t_lexer_err	append_part(t_word ***tail, char *str, int len, uint8_t flag)
{
	t_word	*part;

	if (len == 0 && !(flag & (W_SQ | W_DQ)))
		return (LEX_NO_ERR);
	part = ft_calloc(1, sizeof(t_word));
	if (!part)
		return (LEX_ERR_MEMORY_ALLOCATION);
	part->str = str;
	part->len = len;
	part->flag = flag;
	**tail = part;
	*tail = &part->next;
	return (LEX_NO_ERR);
}

t_lexer_err	commit_part(t_word ***tail, char **begin,
				char *end, uint8_t flag)
{
	if (end <= *begin)
		return (LEX_NO_ERR);
	if (append_part(tail, *begin, end - *begin, flag) != LEX_NO_ERR)
		return (LEX_ERR_MEMORY_ALLOCATION);
	*begin = end;
	return (LEX_NO_ERR);
}

t_lexer_err	close_quote_part(char **line, t_word ***tail,
				char *begin, uint8_t flag)
{
	if (commit_part(tail, &begin, *line, flag) != LEX_NO_ERR)
		return (LEX_ERR_MEMORY_ALLOCATION);
	if (*line == begin && begin[-1] == *begin
		&& append_part(tail, begin, 0, flag) != LEX_NO_ERR)
		return (LEX_ERR_MEMORY_ALLOCATION);
	(*line)++;
	return (LEX_NO_ERR);
}

void	lex_dollar(char **cur_ptr, uint8_t *flag)
{
	(*cur_ptr)++;
	if (**cur_ptr == '?')
	{
		(*cur_ptr)++;
		*flag = W_DOLL;
		return ;
	}
	else if (ft_isalpha(**cur_ptr) || **cur_ptr == '_')
	{
		(*cur_ptr)++;
		while (ft_isalnum(**cur_ptr) || **cur_ptr == '_')
			(*cur_ptr)++;
		*flag = W_DOLL;
	}
}
