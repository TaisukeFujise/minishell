/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 22:44:56 by fendo             #+#    #+#             */
/*   Updated: 2026/02/02 15:36:44 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"
#include "minishell.h"

static int	is_tk_bound(char *p)
{
	return (ft_strchr(" \t\n|()><", *p) 
			|| strchunk("&&||>><<", p, 2));
}

static t_word	*new_part(char *str, int len, uint8_t flag)
{
	t_word	*p;

	p = ft_calloc(1, sizeof(t_word));
	if (p)
	{
		p->str = str;
		p->len = len;
		p->flag = flag;
	}
	return (p);
}

static int	scan_sq(char **line, t_word **tail)
{
	char	*begin;
	int		len;

	begin = ++(*line);
	while (**line && ft_strncmp(*line, "\'", 1) != 0)
		(*line)++;
	if (ft_strncmp(*line, "\'", 1) != 0)
		return (ERR_UNCLOSED_QUOTE);
	len = *line - begin;
	if (len > 0 && !(*tail = new_part(begin, len, W_SQ)))
		return (-1);
	(*line)++;
	return (0);
}

static int	scan_dq(char **line, t_word **tail)
{
	char	*begin;
	char	*dollar;

	begin = ++(*line);
	while (**line && ft_strncmp(*line, "\"", 1) != 0)
	{
		if (ft_strncmp(*line, "$", 1) == 0)
		{
			if (*line > begin)
			{
				if (!(*tail = new_part(begin, *line - begin, W_DQ)))
					return (-1);
				tail = &(*tail)->next;
			}
			dollar = *line;
			(*line)++;
			while (**line && ft_strncmp(*line, "\"", 1) != 0
				&& ft_strncmp(*line, "$", 1) != 0)
				(*line)++;
			if (!(*tail = new_part(dollar, *line - dollar, W_DQ | W_DOLL)))
				return (-1);
			tail = &(*tail)->next;
			begin = *line;
		}
		else
			(*line)++;
	}
	if (ft_strncmp(*line, "\"", 1) != 0)
		return (ERR_UNCLOSED_QUOTE);
	if (*line > begin && !(*tail = new_part(begin, *line - begin, W_DQ)))
		return (-1);
	(*line)++;
	return (0);
}

static int	scan_special(char **line, t_word **tail)
{
	char	*begin;
	uint8_t	flag;

	begin = *line;
	if (ft_strncmp(*line, "$", 1) == 0)
	{
		flag = W_DOLL;
		while (*++(*line) && !is_tk_bound(*line) && !ft_strchr("\'\"$*", **line))
			;
	}
	else if (ft_strncmp(*line, "*", 1) == 0)
	{
		flag = W_WILD;
		while (ft_strncmp(*line, "*", 1) == 0)
			(*line)++;
	}
	else
		return (0);
	*tail = new_part(begin, *line - begin, flag);
	if (*tail == NULL)
		return (-1);
	return (0);
}

static int	scan_plain(char **line, t_word **tail, t_assign_state *as,
				t_token *tk)
{
	char	*begin;

	begin = *line;
	while (**line && !is_tk_bound(*line) && !ft_strchr("\'\"$*", **line))
		validate_assign((*line)++, tk, as);
	if (*line > begin && !(*tail = new_part(begin, *line - begin, W_NONE)))
		return (-1);
	return (0);
}

void	lex_word(char **line, t_token *tk)
{
	t_word			*head;
	t_word			**tail;
	t_assign_state	as;
	int				err;
	t_word			*curr;

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
			err = scan_sq(line, tail);
		else if (ft_strncmp(*line, "\"", 1) == 0)
			err = scan_dq(line, tail);
		else if (ft_strchr("$*", **line))
			err = scan_special(line, tail);
		else
			err = scan_plain(line, tail, &as, tk);
		if (err && free_word_parts(head))
		{
			if (err > 0)
				set_lex_error(tk, err);
			return ;
		}
		if (*tail)
		{
			curr = *tail;
			while (curr->next)
				curr = curr->next;
			tail = &(curr->next);
		}
	}
	tk->token_kind = TK_WORD;
	tk->u_token.wd.next = head;
}
