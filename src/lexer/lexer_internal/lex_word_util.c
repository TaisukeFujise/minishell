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
	if (**line != *quote)
		return (ERR_UNCLOSED_QUOTE);
	else if (append_part(tail, begin, *line - begin, flag) < 0)
		return (-1);
	(*line)++;
	return (0);
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
