/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:15:55 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:15:56 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned char	*str;
	size_t			i;
	int				found;

	str = (unsigned char *)s;
	i = 0;
	found = 0;
	while (str[i] != '\0')
	{
		if (str[i] == (unsigned char)c)
		{
			str = str + i;
			i = 0;
			found = 1;
		}
		i++;
	}
	if (found)
		return ((char *)str);
	if (c == '\0')
		return ((char *)(str + i));
	return (NULL);
}

// #include <stdio.h>
// #include <string.h>

// void	test_ft_strrchr(const char *s, char c)
// {
// 	printf("str        = %s\n", s);
// 	char *tstr = ft_strrchr(s, (int)c);
// 	char *tstr2 = strrchr(s, (int)c);
// 	printf("strrchr    = %s\n", tstr2);
// 	printf("ft_strrchr = %s\n", tstr);
// 	printf("\n");
// }

// int main(void)
// {
// 	test_ft_strrchr("Hello World!", 'o');
// 	test_ft_strrchr("Hello World!", 'l');
// 	test_ft_strrchr("Hello World!", 'x');
// 	test_ft_strrchr("Hello World!", '\0');
// }
