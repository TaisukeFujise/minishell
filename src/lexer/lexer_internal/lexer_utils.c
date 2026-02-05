/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 19:58:11 by fendo             #+#    #+#             */
/*   Updated: 2026/02/05 18:11:25 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexer_internal.h"
#include <limits.h>
#include <stddef.h>

// Refer to section "3.45 Blank Character (<blank>)" 
// in https://pubs.opengroup.org/onlinepubs/9799919799/
void	skip_blank(char **line)
{
	while (**line != '\0' && ft_isblank(**line))
		(*line)++;
}

unsigned int	str2fd(char **line)
{
	long long	num;

	num = 0;
	while (ft_isdigit(**line))
	{
		if (num > (UINT_MAX - (**line - '0')) / 10)
			num = UINT_MAX;
		else
			num = num * 10u + (**line - '0');
		(*line)++;
	}
	return ((unsigned int)num);
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

int	is_tk_bound(char *ch)
{
	return (ft_strchr(" \t\n|()><", *ch) || strchunk("&&||>><<", ch, 2));
}

void	set_lexer_error(t_token *token, int err)
{
	if (!token)
		return ;
	token->token_kind = TK_ERR;
	token->u_token.err = err;
}
