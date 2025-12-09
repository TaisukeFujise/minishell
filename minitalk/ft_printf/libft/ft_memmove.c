/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:15:14 by fendo             #+#    #+#             */
/*   Updated: 2025/10/18 13:46:59 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*d;
	unsigned char	*s;

	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	if (dest < src)
	{
		i = 0;
		while (i < n)
		{
			d[i] = s[i];
			i++;
		}
	}
	else
	{
		while (n-- > 0)
			d[n] = s[n];
	}
	return (dest);
}

// #include <stdio.h>

// void	test_ft_memmove(void *dst, size_t start, size_t n)
// {
// 	printf("src = %s\n", (char *)(dst + start));
// 	printf("start = %lu\n", start);
// 	printf("n     = %lu\n", n);
// 	printf("Before: dst = %s\n", (char *)dst);
// 	char *ndst = (char *)ft_memmove(dst, dst + start, n);
// 	printf("After : dst = %s\n", ndst);
// }

// int main(void)
// {
//     char buf[20] = "1234567890abcdefghi";
// 	test_ft_memmove(buf, 4, 10);
// 	char buf2[20] = "1234567890abcdefghi";
// 	test_ft_memmove(buf2, 1, 14);
//     return 0;
// }