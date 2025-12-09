/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:15:39 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 17:09:58 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t n)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	j;

	dst_len = ft_strlen(dest);
	src_len = ft_strlen((char *)src);
	if (n < dst_len)
		return (n + src_len);
	j = 0;
	while (src[j] != '\0' && dst_len + j + 1 < n)
	{
		dest[dst_len + j] = src[j];
		j++;
	}
	dest[dst_len + j] = '\0';
	return (dst_len + src_len);
}

// #include <stdio.h>
// #include <string.h>

// void	test_ft_strlcat(char *dst, const char *src, size_t size)
// {
// 	size_t n;
// 	printf("dst=\"%s\"\n", dst);
// 	printf("src=\"%s\"\n", src);
// 	printf("size=%lu\n", size);
// 	n = ft_strlcat(dst, src, size);
// 	printf("dst=\"%s\", return=%lu\n", dst, n);
// }

// int main(void)
// {
// 	char dst[20] = "dest";
// 	char src[20] = "source";
// 	size_t n;
// 	test_ft_strlcat(dst, src, 5);
// 	char dst2[20] = "dest";
// 	char src2[20] = "source";
// 	test_ft_strlcat(dst2, src2, 2);
// 	char dst3[20] = "dest";
// 	char src3[20] = "source";
// 	test_ft_strlcat(dst3, src3, 10);
// 	char dst4[20] = "dest";
// 	char src4[20] = "source";
// 	test_ft_strlcat(dst4, src4, 20);
// 	return 0;
// }
