/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:15:04 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:15:05 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	size_t			i;

	str = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (str[i] == (unsigned char)c)
			return ((char *)(str + i));
		i++;
	}
	return (NULL);
}

// #include <stdio.h>
// #include <string.h>

// void test_ft_memchr(const char *str, char c, size_t n)
// {
// 	printf("str = %s\n", str);
// 	char *mstr = ft_memchr(str, (int)c, n);
// 	printf("mstr = %s\n", mstr);
// }

// int main(void)
// {
// 	test_ft_memchr("abcdefg", 'd', 6);
// 	test_ft_memchr("abcdefg", 'x', 5);
// 	return 0;
// }
