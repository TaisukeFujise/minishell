/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_word_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 22:00:38 by fendo             #+#    #+#             */
/*   Updated: 2026/03/02 23:41:37 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"
#include "../../../include/minishell.h"

/*
	lex rule:
	- part node construction (all states)
	- len == 0 && !(W_SQ|W_DQ) -> skip (empty unquoted part)
*/
t_lexer_err	append_part(t_word_builder *wb, char *str, int len, uint8_t flag)
{
	t_word	*part;

	if (len == 0 && !(flag & (W_SQ | W_DQ)))
		return (LEX_NO_ERR);
	part = ft_arena_calloc(wb->arena, 1, sizeof(t_word));
	if (!part)
		return (LEX_ERR_MEMORY_ALLOCATION);
	part->str = str;
	part->len = len;
	part->flag = flag;
	*(wb->tail) = part;
	wb->tail = &part->next;
	return (LEX_NO_ERR);
}

t_lexer_err	commit_part(t_word_builder *wb, char **begin,
						char *end, uint8_t flag)
{
	if (end <= *begin)
		return (LEX_NO_ERR);
	if (append_part(wb, *begin, end - *begin, flag) != LEX_NO_ERR)
		return (LEX_ERR_MEMORY_ALLOCATION);
	*begin = end;
	return (LEX_NO_ERR);
}

t_lexer_err	close_quote_part(t_word_builder *wb, char **line, char *begin,
							uint8_t flag)
{
	if (commit_part(wb, &begin, *line, flag) != LEX_NO_ERR)
		return (LEX_ERR_MEMORY_ALLOCATION);
	if (*line == begin && begin[-1] == *begin
		&& append_part(wb, begin, 0, flag) != LEX_NO_ERR)
		return (LEX_ERR_MEMORY_ALLOCATION);
	(*line)++;
	return (LEX_NO_ERR);
}

/*
	parse.y ref:
	- read_token_word: '$' handling (L5507-L5567)
	lex rule:
	- \$NAME  -> W_DOLL
	- \$\?      -> W_DOLL
	- \$        -> W_NONE (bare dollar)
*/
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
