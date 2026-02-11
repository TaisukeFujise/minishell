/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 21:43:37 by fendo             #+#    #+#             */
/*   Updated: 2026/02/10 22:20:55 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"
#include "minishell.h"

static int	scan_sq(char **line, t_word ***tail)
{
	char	*begin;

	begin = ++(*line);
	while (**line && **line != '\'')
		(*line)++;
	return (finish_quote(line, tail, begin, W_SQ));
}

static int	scan_dq(char **line, t_word ***tail)
{
	char	*begin;
	char	*dollar;
	uint8_t	flag;

	begin = ++(*line);
	while (**line && **line != '\"')
	{
		if (**line == '$')
		{
			if (append_part(tail, begin, *line - begin, W_DQ) < 0)
				return (-1);
			dollar = *line;
			flag = W_NONE;
			lex_dollar(line, &flag);
			if (append_part(tail, dollar, *line - dollar, W_DQ | flag) < 0)
				return (-1);
			begin = *line;
		}
		else
			(*line)++;
	}
	return (finish_quote(line, tail, begin, W_DQ));
}

static int	scan_unquoted(char **line, t_word ***tail, t_assign_info *as)
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
			validate_assign((*line)++, as);
	if (append_part(tail, begin, *line - begin, flag) < 0)
		return (-1);
	return (0);
}

static int	scan_word(char **line, t_word **head, t_assign_info *as)
{
	t_word	**tail;
	int		err;

	tail = head;
	while (**line && !is_tk_bound(*line))
	{
		if (ft_strchr("\'\"$*", **line))
			validate_assign(*line, as);
		if (**line == '\'')
			err = scan_sq(line, &tail);
		else if (**line == '\"')
			err = scan_dq(line, &tail);
		else
			err = scan_unquoted(line, &tail, as);
		if (err)
			return (err);
	}
	return (0);
}

t_token_kind	lex_word(char **line, t_token *tk)
{
	t_word			*head;
	t_assign_info	as;
	int				err;

	head = NULL;
	set_assign_info(&as, AS_INIT, NULL, W_NONE);
	err = scan_word(line, &head, &as);
	if (err)
	{
		free_word_parts(head);
		if (err > 0)
			set_lexer_error(tk, err);
		return (tk->token_kind);
	}
	tk->token_kind = TK_WORD;
	tk->u_token.wd = head;
	apply_assign_info(head, &as);
	return (tk->token_kind);
}
