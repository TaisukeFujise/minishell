/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 19:58:11 by fendo             #+#    #+#             */
/*   Updated: 2026/01/08 16:40:38 by fendo            ###   ########.fr       */
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

void	validate_assign(char *cur_ptr, t_token *token, t_assign_state *as)
{
	if (*as == AS_DONE || *as == AS_INVALID)
		return ;
	if (!ft_strncmp(cur_ptr, "=", 1))
	{
		if (*as == AS_VALID)
		{
			token->u_token.wd.eq_ptr = cur_ptr;
			token->u_token.wd.flag |= W_ASSIGN;
		}
		*as = AS_DONE;
		return ;
	}
	if (*as == AS_INIT)
	{
		if (ft_isalpha(*cur_ptr) || !ft_strncmp(cur_ptr, "_", 1))
			*as = AS_VALID;
		else
			*as = AS_INVALID;
	}
	else
	{
		if (!(ft_isalnum(*cur_ptr) || !ft_strncmp(cur_ptr, "_", 1)))
			*as = AS_INVALID;
	}
}
