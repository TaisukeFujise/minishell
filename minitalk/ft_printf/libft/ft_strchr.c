/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 14:01:02 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:15:31 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	*str;

	str = (unsigned char *)s;
	while (*str != '\0')
	{
		if (*str == (unsigned char)c)
			return ((char *)str);
		str++;
	}
	if (*str == '\0' && c == '\0')
		return ((char *)str);
	return (NULL);
}

// #include <stdio.h>
// #include <string.h>

// void	test_ft_strchr(const char *s, char c)
// {
// 	printf("str       = %s\n", s);
// 	char *tstr = ft_strchr(s, (int)c);
// 	printf("ft_strchr = %s\n", tstr);
// }

// int main(void)
// {
// 	test_ft_strchr("Hello World!", 'o');
// 	test_ft_strchr("Hello World!", 'l');
// 	test_ft_strchr("Hello World!", 'x');
// 	test_ft_strchr("Hello World!", '\0');
// }
