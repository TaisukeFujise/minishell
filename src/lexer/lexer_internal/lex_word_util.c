/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_word_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 22:00:38 by fendo             #+#    #+#             */
/*   Updated: 2026/02/02 22:02:24 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"
#include "minishell.h"

int	is_tk_bound(char *ch)
{
	return (ft_strchr(" \t\n|()><", *ch) || strchunk("&&||>><<", ch, 2));
}

void	init_lex(t_word	*head, t_word	**tail,
				t_assign_state	*as, t_token	*tk)
{
	head = NULL;
	tail = &head;
	*as = AS_INIT;
	tk->u_token.wd.flag = W_NONE;
	tk->u_token.wd.eq_ptr = NULL;
}

int	append_part(t_word ***tail, char *str, int len, uint8_t flag)
{
	t_word	*part;

	if (len <= 0)
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
