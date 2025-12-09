/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 08:36:24 by tafujise          #+#    #+#             */
/*   Updated: 2025/10/17 22:08:25 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	little_len;

	little_len = ft_strlen(little);
	if (*little == '\0')
		return ((char *)big);
	while (*big != '\0' && len > 0)
	{
		if (!ft_strncmp(big, little, little_len) && len >= little_len)
			return ((char *)big);
		big++;
		len--;
	}
	return (0);
}
