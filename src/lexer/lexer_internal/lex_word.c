/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 21:43:37 by fendo             #+#    #+#             */
/*   Updated: 2026/02/12 00:23:02 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"
#include "minishell.h"

static t_lexer_err	scan_sq(char **line, t_word ***tail)
{
	char	*begin;

	begin = ++(*line);
	ft_strspan(line, "\'", false);
	if (**line != '\'')
		return (LEX_ERR_UNCLOSED_SINGLE_QUOTE);
	return (close_quote_part(line, tail, begin, W_SQ));
}

static t_lexer_err	scan_dq(char **line, t_word ***tail)
{
	char	*begin;
	uint8_t	flag;

	begin = ++(*line);
	while (**line && **line != '\"')
	{
		if (**line == '$')
		{
			flag = W_NONE;
			lex_dollar(line, &flag);
			if (commit_part(tail, &begin, *line, W_DQ | flag) != LEX_NO_ERR)
				return (LEX_ERR_MEMORY_ALLOCATION);
		}
		else
		{
			ft_strspan(line, "\"$", false);
			if (commit_part(tail, &begin, *line, W_DQ) != LEX_NO_ERR)
				return (LEX_ERR_MEMORY_ALLOCATION);
		}
	}
	if (**line != '\"')
		return (LEX_ERR_UNCLOSED_DOUBLE_QUOTE);
	return (close_quote_part(line, tail, begin, W_DQ));
}

static t_lexer_err	scan_unquoted(char **line, t_word ***tail,
						t_assign_info *as)
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
		ft_strspan(line, "*", true);
	}
	else
		while (**line && !is_tk_bound(*line) && !ft_strchr("\'\"$*", **line))
			validate_assign((*line)++, as);
	return (commit_part(tail, &begin, *line, flag));
}

static t_lexer_err	scan_word(char **line, t_word **head, t_assign_info *as)
{
	t_word		**tail;
	t_lexer_err	err;

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
	return (LEX_NO_ERR);
}

t_token_kind	lex_word(char **line, t_token *tk)
{
	t_word			*head;
	t_assign_info	as;
	t_lexer_err		err;

	head = NULL;
	set_assign_info(&as, AS_INIT, NULL, W_NONE);
	err = scan_word(line, &head, &as);
	if (err)
	{
		free_word_parts(head);
		if (err != LEX_ERR_MEMORY_ALLOCATION)
			set_lexer_error(tk, err);
		return (tk->token_kind);
	}
	tk->token_kind = TK_WORD;
	tk->u_token.wd = head;
	apply_assign_info(head, &as);
	return (tk->token_kind);
}
