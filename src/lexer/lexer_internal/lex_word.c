/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 21:43:37 by fendo             #+#    #+#             */
/*   Updated: 2026/02/02 18:02:42 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"
#include "minishell.h"

static int	is_tk_bound(char *p)
{
	return (ft_strchr(" \t\n|()><", *p) || strchunk("&&||>><<", p, 2));
}

static int	append_part(t_word ***tail, char *str, int len, uint8_t flag)
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

static int	finish_quote(char **line, t_word ***tail, char *begin, uint8_t flag)
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

static int	scan_special(char **line, t_word ***tail)
{
	char	*begin;
	uint8_t	flag;

	begin = *line;
	if (ft_strncmp(*line, "$", 1) == 0)
	{
		flag = W_DOLL;
		while (**line && !is_tk_bound(*line) && !ft_strchr("\'\"$*", **line))
			(*line)++;
	}
	else if (ft_strncmp(*line, "*", 1) == 0)
	{
		flag = W_WILD;
		while (ft_strncmp(*line, "*", 1) == 0)
			(*line)++;
	}
	else
		return (0);
	if (append_part(tail, begin, *line - begin, flag) < 0)
		return (-1);
	return (0);
}

static int	scan_plain(char **line, t_word ***tail, t_assign_state *as,
				t_token *tk)
{
	char	*begin;

	begin = *line;
	while (**line && !is_tk_bound(*line) && !ft_strchr("\'\"$*", **line))
		validate_assign((*line)++, tk, as);
	if (append_part(tail, begin, *line - begin, W_NONE) < 0)
		return (-1);
	return (0);
}

void	lex_word(char **line, t_token *tk)
{
	t_word			*head;
	t_word			**tail;
	t_assign_state	as;
	int				err;

	head = NULL;
	tail = &head;
	as = AS_INIT;
	tk->u_token.wd.flag = W_NONE;
	tk->u_token.wd.eq_ptr = NULL;
	while (**line && !is_tk_bound(*line))
	{
		if (ft_strchr("\'\"$*", **line))
			validate_assign(*line, tk, &as);
		if (ft_strncmp(*line, "\'", 1) == 0)
			err = scan_sq(line, &tail);
		else if (ft_strncmp(*line, "\"", 1) == 0)
			err = scan_dq(line, &tail);
		else if (ft_strchr("$*", **line))
			err = scan_special(line, &tail);
		else
			err = scan_plain(line, &tail, &as, tk);
		if (err && free_word_parts(head))
		{
			if (err > 0)
				set_lex_error(tk, err);
			return ;
		}
	}
	tk->token_kind = TK_WORD;
	tk->u_token.wd.next = head;
}
