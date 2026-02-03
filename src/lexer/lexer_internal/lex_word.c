/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 21:43:37 by fendo             #+#    #+#             */
/*   Updated: 2026/02/03 20:50:18 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"
#include "minishell.h"

static int	scan_sq(char **line, t_word ***tail)
{
	char	*begin;

	begin = ++(*line);
	while (**line && ft_strncmp(*line, "\'", 1) != 0)
		(*line)++;
	return (finish_quote(line, tail, begin, W_SQ));
}

static int	scan_dq(char **line, t_word ***tail)
{
	char	*begin;
	char	*dollar;

	begin = ++(*line);
	while (**line && ft_strncmp(*line, "\"", 1) != 0)
	{
		if (ft_strncmp(*line, "$", 1) == 0)
		{
			if (append_part(tail, begin, *line - begin, W_DQ) < 0)
				return (-1);
			dollar = *line;
			(*line)++;
			while (**line && !ft_strchr("\"$", **line))
				(*line)++;
			if (append_part(tail, dollar, *line - dollar, W_DQ | W_DOLL) < 0)
				return (-1);
			begin = *line;
		}
		else
			(*line)++;
	}
	return (finish_quote(line, tail, begin, W_DQ));
}

static int	scan_unquoted(char **line, t_word ***tail,
				t_assign_state *as, t_token *tk)
{
	char	*begin;
	uint8_t	flag;

	begin = *line;
	flag = W_NONE;
	if (**line == '$')
		lex_dollar(line, &flag);
	else if (**line == '*')
	{
		flag = W_WILD;
		while (**line == '*')
			(*line)++;
	}
	else
		while (**line && !is_tk_bound(*line) && !ft_strchr("\'\"$*", **line))
			validate_assign((*line)++, tk, as);
	if (append_part(tail, begin, *line - begin, flag) < 0)
		return (-1);
	return (0);
}

static int	dispatch_word_lexing(t_word **tail, char **line,
								t_token *tk, t_assign_state *as)
{
	int	err;

	if (ft_strncmp(*line, "\'", 1) == 0)
		err = scan_sq(line, &tail);
	else if (ft_strncmp(*line, "\"", 1) == 0)
		err = scan_dq(line, &tail);
	else if (ft_strchr("$*", **line))
		err = scan_special(line, &tail);
	else
		err = scan_plain(line, &tail, as, tk);
	return (err);
}

t_token_kind	lex_word(char **line, t_token *tk)
{
	t_word			*head;
	t_word			**tail;
	t_assign_state	as;
	int				err;

	init_lex(&head, &tail, &as, tk);
	while (**line && !is_tk_bound(*line))
	{
		if (ft_strchr("\'\"$*", **line))
			validate_assign(*line, tk, &as);
		err = dispatch_word_lexing(tail, line, tk, &as);
		if (err && free_word_parts(head))
		{
			if (err > 0)
				set_lex_error(tk, err);
			return (tk->token_kind);
		}
	}
	tk->token_kind = TK_WORD;
	tk->u_token.wd.next = head;
	return (tk->token_kind);
}
