/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:15:11 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:15:12 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char			*d;
	const unsigned char		*s;
	size_t					i;

	if (n == 0)
		return (dst);
	if (!dst || !src)
		return (NULL);
	d = (unsigned char *)dst;
	s = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

// #include <stdio.h>

// void	test_ft_memcpy(char *dst, const char *src, size_t n)
// {
// 	printf("src = %s\n", src);
// 	printf("n   = %lu\n", n);
// 	printf("Before: dst = %s\n", dst);
// 	char *ndst = (char *)ft_memcpy(dst, src, n);
// 	printf("After : dst = %s\n", ndst);
// }

// int main(void)
// {
// 	char src[] = "123456789";
// 	char dst[] = "abcdefghijk";
// 	int n = 8;
// 	test_ft_memcpy(dst, src, n);
// 	char src2[] = "123456789";
// 	char dst2[] = "abcdefghijk";
// 	n = 0;
// 	test_ft_memcpy(dst2, src2, n);
// 	return 0;
// }