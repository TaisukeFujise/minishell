/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 20:59:07 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/14 23:06:22 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*src;
	char	*dst;
	size_t	i;

	if (!s)
		return (NULL);
	src = (char *)s;
	if (ft_strlen(src) > n)
	{
		dst = malloc(sizeof(char) * (n + 1));
		if (!dst)
			return (NULL);
		i = 0;
		while (i < n)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
		return (dst);
	}
	else
		return (ft_strdup(src));
}

// #include <stdio.h>
// #include <string.h>

// int	main(void)
// {
// 	char	*str;

// 	str = ft_strndup("abc", 5);
// 	printf("%s\n", str);
// 	str = strndup("abc", 5);
// }
