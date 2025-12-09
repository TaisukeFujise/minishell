/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:15:17 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:15:18 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*b_char;

	i = 0;
	b_char = (unsigned char *)s;
	while (i < n)
	{
		b_char[i] = (unsigned char)c;
		i++;
	}
	return (s);
}

// #include <stdio.h>

// void	test_ft_memset(char *str, int n, int len)
// {
// 	printf("n   = %c\n", n);
// 	printf("len = %d\n", len);
// 	printf("Before: str = %s\n", str);
// 	char *mstr = (char *)ft_memset(str, n, len);
// 	printf("After : str = %s\n", mstr);
// }

// int main(void)
// {
// 	char str[] = "123456789";
// 	int n = '0';
// 	int len = 5;
// 	test_ft_memset(str, n, len);
// 	char str2[] = "abcdefghi";
// 	n = 'X';
// 	len = 3;
// 	test_ft_memset(str2, n, len);
// 	return 0;
// }