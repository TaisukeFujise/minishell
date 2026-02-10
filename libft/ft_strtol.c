/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 04:14:40 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/11 04:43:36 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	round_long_limits(long long num)
{
	printf("round num: %lld\n", num);
	if (num < LONG_MIN)
	{
		errno = ERANGE;
		return (LONG_MIN);
	}
	if (LONG_MAX < num)
	{
		errno = ERANGE;
		return (LONG_MAX);
	}
	return ((long)num);
}

long	ft_strtol(const char *nptr)
{
	int			sign;
	long long	num;

	sign = 1;
	while (ft_isspace(*nptr))
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign *= -1;
		nptr++;
	}
	if (ft_isdigit(*nptr) == 0)
		return (0);
	num = *nptr - '0';
	nptr++;
	while (ft_isdigit(*nptr))
	{
		num = num * 10 + (*nptr - '0');
		if (LONG_MAX < num)
			return (round_long_limits(num *sign));
		nptr++;
	}
	return (round_long_limits(num * sign));
}

// #include <errno.h>

// int	main(void)
// {
// 	char *str = "876543239397";
// 	printf("%ld\n", ft_strtol(str));
// }
