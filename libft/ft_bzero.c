/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 18:08:19 by fujisetaisu       #+#    #+#             */
/*   Updated: 2025/10/17 22:06:36 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*dest;

	dest = (char *)s;
	i = 0;
	if (n == 0)
		return ;
	while (i < n)
	{
		dest[i] = 0;
		i++;
	}
	return ;
}
