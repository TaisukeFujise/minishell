/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 19:58:11 by fendo             #+#    #+#             */
/*   Updated: 2025/12/24 17:30:21 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
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
			num = num * 10u + (*(*line)++ - '0');
	}
	return ((unsigned int)num);
}

// extended version of ft_strchr: search for a substring 'str' of length n in 'tbl'
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
