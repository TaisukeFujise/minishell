/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:15:41 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 17:10:04 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	if (n == 0)
		return (ft_strlen((char *)src));
	i = 0;
	while (i + 1 < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (ft_strlen((char *)src));
}

// #include <stdio.h>

// void	test_ft_strlcpy(char *dst, const char *src, size_t size)
// {
// 	size_t n;
// 	printf("dst=\"%s\"\n", dst);
// 	n = ft_strlcpy(dst, src, size);
// 	printf("dst=\"%s\", return=%lu\n", dst, n);
// }

// int main(void)
// {
// 	char dst[20];
// 	const char *src = "HelloWorld";
// 	size_t size = 14;
// 	test_ft_strlcpy(dst, src, size);
// 	char dst2[20];
// 	const char *src2 = "HelloWorld";
// 	size_t size2 = 5;
// 	test_ft_strlcpy(dst2, src2, size2);
// 	return 0;
// }