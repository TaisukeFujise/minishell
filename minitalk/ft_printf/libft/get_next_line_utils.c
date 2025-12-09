/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 18:56:32 by fendo             #+#    #+#             */
/*   Updated: 2025/10/31 15:51:10 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*ft_calloc_gnl(size_t count, size_t size)
{
	size_t			i;
	size_t			bytes;
	unsigned char	*arr;

	if (count == 0 || size == 0)
	{
		return (malloc(0));
	}
	if (size != 0 && count > SIZE_MAX / size)
		return (NULL);
	i = 0;
	bytes = count * size;
	arr = malloc(bytes);
	if (!arr)
		return (NULL);
	while (i < bytes)
	{
		arr[i] = 0;
		i++;
	}
	return (arr);
}

char	*ft_strchr_gnl(const char *s, int c)
{
	unsigned char	*str;

	if (!s)
		return (NULL);
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

char	*ft_strndup_gnl(const char *s1, size_t n)
{
	char	*s2;
	size_t	i;

	if (!s1)
		return (NULL);
	i = 0;
	while (i < n && s1[i] != '\0')
		i++;
	s2 = (char *)malloc(sizeof(char) * (i + 1));
	if (!s2)
		return (NULL);
	i = 0;
	while (i < n && s1[i] != '\0')
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}

char	*ft_strjoin_gnl(char *s1, const char *s2)
{
	char	*ans;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	ans = (char *)malloc(ft_strlen_gnl(s1) + ft_strlen_gnl(s2) + 1);
	if (ans == NULL)
	{
		free(s1);
		return (NULL);
	}
	i = 0;
	j = 0;
	while (s1[i] != '\0')
		ans[j++] = s1[i++];
	i = 0;
	while (s2[i] != '\0')
		ans[j++] = s2[i++];
	ans[j] = '\0';
	free((void *)s1);
	return (ans);
}

size_t	ft_strlen_gnl(const char *s)
{
	size_t	n;

	if (!s)
		return (0);
	n = 0;
	while (s[n] != '\0')
		n++;
	return (n);
}
