/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 04:14:40 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/11 10:51:26 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long	_consume_symbol(char **nptr);

long	ft_atol(char *nptr)
{
	long				sign;
	unsigned long long	acc;
	unsigned long long	limit;

	sign = _consume_symbol(&nptr);
	acc = 0;
	if (sign == 1)
		limit = (unsigned long long)LONG_MAX;
	else
		limit = (unsigned long long)LONG_MIN;
	while (ft_isdigit(*nptr))
	{
		if (acc > ((limit - (*nptr - '0')) / 10))
		{
			errno = ERANGE;
			return (limit);
		}
		acc = acc * 10 + (*nptr - '0');
		nptr++;
	}
	return (sign * (long)acc);
}

static long	_consume_symbol(char **nptr)
{
	while (ft_isspace(**nptr))
		(*nptr)++;
	if (**nptr == '+')
	{
		(*nptr)++;
		return (1);
	}
	else if (**nptr == '-')
	{
		(*nptr)++;
		return (-1);
	}
	return (1);
}

// #include <errno.h>
// #include <stdio.h>

// int	main(void)
// {
// 	char *str = "-0";
// 	printf("%ld\n", ft_atol(str));
// }
