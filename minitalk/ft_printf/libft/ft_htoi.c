/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_htoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 15:23:00 by fendo             #+#    #+#             */
/*   Updated: 2025/12/01 15:23:01 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_htoi(const char *hex)
{
	int	result;
	int	i;
	int	value;

	result = 0;
	i = 0;
	while (i < 6)
	{
		if (hex[i] >= '0' && hex[i] <= '9')
			value = hex[i] - '0';
		else if (hex[i] >= 'A' && hex[i] <= 'F')
			value = hex[i] - 'A' + 10;
		else if (hex[i] >= 'a' && hex[i] <= 'f')
			value = hex[i] - 'a' + 10;
		else
			break ;
		result = result * 16 + value;
		i++;
	}
	return (result);
}
