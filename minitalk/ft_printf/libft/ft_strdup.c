/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:15:32 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 15:15:33 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*s2;
	size_t	i;

	if (s == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
		i++;
	s2 = (char *)malloc(sizeof(char) * (i + 1));
	i = 0;
	while (s[i] != '\0')
	{
		s2[i] = s[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}

// #include <stdio.h>
// #include <string.h>

// void test_ft_strdup(const char *s)
// {
// 	char *dup = ft_strdup(s);
// 	char *orig = strdup(s);
// 	printf("s    = \"%s\"\n", s);
// 	printf("dup  = \"%s\"\n", dup);
// 	printf("orig = \"%s\"\n", orig);
// 	free(dup);
// 	free(orig);
// }

// int main(void)
// {
// 	test_ft_strdup("Hello, World!");
// 	test_ft_strdup("");
// 	return 0;
// }