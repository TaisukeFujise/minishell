/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 08:13:37 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/20 20:15:27 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	round_int_limits(long int num)
{
	if (num < INT_MIN)
		return (INT_MIN);
	if (INT_MAX < num)
		return (INT_MAX);
	return ((int)num);
}

int	ft_atoi(const char *nptr)
{
	int			symbol;
	long int	num;

	symbol = 1;
	while (ft_isspace(*nptr))
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			symbol *= -1;
		nptr++;
	}
	if (ft_isdigit(*nptr) == 0)
		return (0);
	num = *nptr - '0';
	nptr++;
	while (ft_isdigit(*nptr))
	{
		num = num * 10 + (*nptr - '0');
		nptr++;
	}
	return (round_int_limits(num * symbol));
}
