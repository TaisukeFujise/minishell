/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 14:05:25 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:15:52 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (*little == '\0')
		return ((char *)big);
	i = 0;
	while (i < len && big[i] != '\0')
	{
		j = 0;
		while (i + j < len && little[j] != '\0' && big[i + j] == little[j])
			j++;
		if (little[j] == '\0')
			return ((char *)(big + i));
		i++;
	}
	return (NULL);
}

// #include <stdio.h>
// #include <string.h>

// void	test_ft_strnstr(char *haystack, char *needle, size_t len)
// {
// 	printf("haystack = %s\n", haystack);
// 	printf("needle   = %s\n", needle);
// 	printf("len      = %zu\n", len);
// 	char *mstr = ft_strnstr(haystack, needle, len);
// 	printf("ft_strnstr = %s\n", mstr);
// }

// int main(void)
// {
// 	test_ft_strnstr("Hello World!", "World!", 11);
// 	test_ft_strnstr("Hello World!", "World!", 12);
// 	test_ft_strnstr("Hello World!", "World!", 15);
// 	test_ft_strnstr("Hello World!", "Hello", 5);
// 	test_ft_strnstr("Hello World!", "Hello", 4);
// 	test_ft_strnstr("Hello World!", "", 5);
// 	test_ft_strnstr("Hello World!", "o W", 10);
// 	return 0;
// }