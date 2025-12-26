/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 19:58:11 by fendo             #+#    #+#             */
/*   Updated: 2025/12/26 22:46:08 by fendo            ###   ########.fr       */
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
// Search for a substring 'str' of length n in 'tbl'
char	*strchunk(const char *tbl, const char *str, size_t n)
{
	size_t	i;
	size_t	len_tbl;

	if (!tbl || !str || n == 0)
		return (NULL);
	i = 0;
	len_tbl = ft_strlen(tbl);
	while (i + n <= len_tbl)
	{
		if (ft_strncmp(tbl + i, str, n) == 0)
			return ((char *)(tbl + i));
		i += n;
	}
	return (NULL);
}

void	scan_asgn(char c, t_token *token, uint8_t *as)
{
	return ;
}
