/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:16:00 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 17:10:31 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	str_len;
	char	*str;

	if (start < (unsigned int)ft_strlen((char *)s))
		str_len = ft_strlen((char *)(s + start));
	else
		str_len = 0;
	if (str_len < len)
		len = str_len;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = s[i + start];
		i++;
	}
	str[i] = '\0';
	return (str);
}

// #include <stdio.h>
// #include <string.h>

// void	test_ft_substr(char const *s, unsigned int start, size_t len)
// {
// 	printf("s     = \"%s\"\n", s);
// 	printf("start = %u\n", start);
// 	printf("len   = %zu\n", len);
// 	char *sub = ft_substr(s, start, len);
// 	printf("sub   = \"%s\"\n", sub);
// 	free(sub);
// 	printf("\n");
// }

// int main(void)
// {
// 	test_ft_substr("Hello, World!", 7, 5);
// 	test_ft_substr("Hello, World!", 0, 5);
// 	test_ft_substr("Hello, World!", 5, 20);
// 	test_ft_substr("Hello, World!", 20, 5);
// 	test_ft_substr("", 0, 5);
// 	return (0);
// }
