/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:14:05 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:14:06 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*s_char;

	i = 0;
	s_char = (unsigned char *)s;
	while (i < n)
	{
		s_char[i] = 0;
		i++;
	}
}

// #include <stdio.h>

// void test_ft_bzero(char *str, int start, int len, int n)
// {
// 	char buf[n];
// 	for (int i = 0; i < n; i++) {
// 		if (i < n - 1 && str[i] != '\0')
// 			buf[i] = str[i];
// 		else
// 			buf[i] = 0;
// 	}
// 	printf("start = %d, len = %d\n", start, len);
// 	printf("Before: ");
// 	for (int i = 0; i < n; i++) {
// 		if (buf[i] == 0)
// 			printf("\\0");
// 		else
// 			printf(" %c", buf[i]);
// 	}
// 	printf("\n");
// 	ft_bzero(buf + start, len);
// 	printf("After : ");
// 	for (int i = 0; i < n; i++) {
// 		if (buf[i] == 0)
// 			printf("\\0");
// 		else
// 			printf(" %c", buf[i]);
// 	}
// 	printf("\n");
// }

// int main(void)
// {
// 	printf("Testing ft_bzero:\n");
// 	test_ft_bzero("abcdefghi", 3, 3, 11);
// 	test_ft_bzero("123456789", 0, 5, 10);
// 	test_ft_bzero("Hello, World!", 7, 6, 15);
// 	printf("\n");
// 	return 0;
// }
