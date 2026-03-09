/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 19:58:11 by fendo             #+#    #+#             */
/*   Updated: 2026/03/02 23:40:17 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../libft/libft.h"
#include "lexer_internal.h"
#include <limits.h>
#include <stddef.h>

/*
	parse.y ref:
	- read_token_word: all_digit_token / NUMBER (L5731-L5741)
	lex rule:
	- DIGIT+ -> int (overflow -> -1)
*/
int	str2fd(char **line)
{
	long long	num;
	int			digit;

	num = 0;
	while (ft_isdigit(**line))
	{
		digit = **line - '0';
		if (num > INT_MAX / 10 || (num == INT_MAX / 10 && digit > INT_MAX % 10))
			return (-1);
		num = num * 10 + digit;
		(*line)++;
	}
	return ((int)num);
}

// Extended version of ft_strchr
// Search for a substring 'str' of length n in 'table'
char	*strchunk(const char *table, const char *str, size_t n)
{
	size_t	i;
	size_t	len_table;

	if (!table || !str || n == 0)
		return (NULL);
	i = 0;
	len_table = ft_strlen(table);
	while (i + n <= len_table)
	{
		if (ft_strncmp(table + i, str, n) == 0)
			return ((char *)(table + i));
		i += n;
	}
	return (NULL);
}

/*
	lex rule:
	- BOUND  = [ \t\n|()><]
	- DBOUND = (&&|\|\||>>|<<)
*/
int	is_tk_bound(char *ch)
{
	return (ft_strchr(" \t\n|()><", *ch) || strchunk("&&||>><<", ch, 2));
}

void	set_lexer_error(t_token *token, t_lexer_err err)
{
	if (!token)
		return ;
	token->token_kind = TK_ERR;
	token->u_token.err = err;
}
