/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:15:08 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:15:09 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	size_t			i;

	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (i < n && str1[i] == str2[i])
		i++;
	if (i == n)
		return (0);
	return (str1[i] - str2[i]);
}

// #include <stdio.h>
// #include <string.h>

// void	test_ft_memcmp(const char *s1, const char *s2, size_t n)
// {
// 	printf("s1 = %s\n", s1);
// 	printf("s2 = %s\n", s2);
// 	printf("n  = %lu\n", n);
// 	int r1 = ft_memcmp(s1, s2, n);
// 	int r2 = memcmp(s1, s2, n);
// 	printf("ft_memcmp = %d\n", r1);
// 	printf("memcmp    = %d\n", r2);
// }

// int main(void)
// {
// 	test_ft_memcmp("Helloworld", "HelloWorld", 6);
// 	test_ft_memcmp("Helloworld", "HelloWorld", 4);
// 	test_ft_memcmp("abcdefg", "abcdefg", 7);
// 	test_ft_memcmp("abcdefg", "abcdeFg", 7);
// 	test_ft_memcmp("abcdefg", "abcdefg", 0);
// }
