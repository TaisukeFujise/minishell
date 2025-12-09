/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:15:49 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:15:50 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && s1[i] != '\0' && s1[i] == s2[i])
		i++;
	if (i == n)
		return (0);
	return (s1[i] - s2[i]);
}

// #include <stdio.h>
// #include <string.h>

// void test_ft_strncmp(void)
// {
// 	char *str1 = "Hello";
// 	char *str2 = "HelloWorld";
// 	printf("str1 - str2 = %d\n", ft_strncmp(str1, str2, 5));
// 	printf("str1 - str2 = %d\n", strncmp(str1, str2, 5));
// }

// int main(void)
// {
// 	test_ft_strncmp();
// 	return 0;
// }
