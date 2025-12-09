/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:15:28 by fendo             #+#    #+#             */
/*   Updated: 2025/10/16 16:32:09 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(const char *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
			count++;
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	return (count);
}

static char	*ft_strndup(const char *s1, size_t n)
{
	char	*s2;
	size_t	i;

	i = 0;
	while (i < n && s1[i] != '\0')
		i++;
	s2 = (char *)malloc(sizeof(char) * (i + 1));
	i = 0;
	while (i < n && s1[i] != '\0')
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}

static int	append_word(char **result, const char *s, size_t i, char c)
{
	size_t	start;

	start = i;
	while (s[i] != c && s[i] != '\0')
		i++;
	*result = ft_strndup(s + start, i - start);
	if (!*result)
	{
		free(*result);
		*result = NULL;
		return (0);
	}
	return (i);
}

static void	free_all(char **result, size_t j)
{
	size_t	i;

	i = 0;
	while (i < j)
	{
		free(result[i]);
		i++;
	}
	free(result);
}

char	**ft_split(const char *s, char c)
{
	char		**result;
	size_t		i;
	size_t		j;

	result = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c && s[i] != '\0')
			i++;
		if (s[i] == '\0')
			break ;
		i = append_word(&result[j], s, i, c);
		if (i == 0)
		{
			free_all(result, j);
			return (NULL);
		}
		j++;
	}
	result[j] = NULL;
	return (result);
}

// #include <string.h>
// #include <stdio.h>

// void free_split(char **split)
// {
// 	if (!split) return;
// 	for (size_t i = 0; split[i] != NULL; i++) {
// 		free(split[i]);
// 	}
// 	free(split);
// }

// void	test_ft_split(const char *s, char c)
// {
// 	printf("s = \"%s\", c = '%c'\n", s, c);
// 	char **split = ft_split(s, c);
// 	if (!split) {
// 		printf("ft_split returned NULL\n");
// 		return;
// 	}
// 	for (size_t i = 0; split[i] != NULL; i++) {
// 		printf("split[%zu] = \"%s\"\n", i, split[i]);
// 	}
// 	free_split(split);
// }

// int main(void)
// {
// 	test_ft_split("  Hello   World  This is   a test  ", ' ');
// 	test_ft_split("one,two,,three,,four", ',');
// 	test_ft_split(",,,start and end,,,", ',');
// 	return (0);
// }
