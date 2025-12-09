/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:15:45 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 17:10:12 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(const char *s, char (*f)(unsigned int, char))
{
	char	*ans;
	size_t	i;

	if (!s || !f)
		return (NULL);
	ans = (char *)malloc(sizeof(char) * (ft_strlen((char *)s) + 1));
	if (!ans)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		ans[i] = f(i, s[i]);
		i++;
	}
	ans[i] = '\0';
	return (ans);
}

// #include <stdio.h>
// #include <stdlib.h>

// char add_i(unsigned int i, char c)
// {
// 	return c + i;
// }

// void	test_ft_strmapi(void)
// {
// 	char *s = "abcdef";
// 	char *res = ft_strmapi(s, add_i);
// 	printf("s   = \"%s\"\n", s);
// 	printf("res = \"%s\"\n", res);
// 	free(res);
// }

// int main(void)
// {
// 	test_ft_strmapi();
// 	return 0;
// }
