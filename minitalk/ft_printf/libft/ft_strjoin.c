/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:15:36 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 17:09:52 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*ans;
	int		i;
	int		j;

	ans = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (ans == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i] != '\0')
		ans[j++] = s1[i++];
	i = 0;
	while (s2[i] != '\0')
		ans[j++] = s2[i++];
	ans[j] = '\0';
	return (ans);
}

// #include <stdio.h>
// #include <string.h>

// void	test_ft_strjoin(void)
// {
// 	char *dst = "Hello ";
// 	char *src = "World!";

// 	printf("dst=\"%s\"\n", dst);
// 	printf("src=\"%s\"\n", src);
// 	char *ans = ft_strjoin(dst, src);
// 	printf("dst + src =\"%s\"\n", ans);
// }

// int main(void)
// {
// 	test_ft_strjoin();
// 	return 0;
// }
