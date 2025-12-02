/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 11:04:07 by tafujise          #+#    #+#             */
/*   Updated: 2025/11/20 21:10:33 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c)
{
	size_t	count;
	int		in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

char	*ft_strndup(const char *src, size_t word_len)
{
	char	*ptr;

	ptr = (char *)malloc(sizeof(char) * (word_len + 1));
	if (!ptr)
		return (NULL);
	ft_strlcpy(ptr, src, word_len + 1);
	return (ptr);
}

static void	move_ptr_by_one_word(char const **s, size_t *word_len, char c)
{
	while (**s != c && **s)
	{
		(*s)++;
		(*word_len)++;
	}
	while (**s == c && **s)
		(*s)++;
}

static int	handle_mem_alloc_failure(char ***res, size_t i)
{
	if ((*res)[i] == NULL)
	{
		while (i > 0)
		{
			i--;
			free((*res)[i]);
		}
		free(*res);
		return (0);
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	char	*start;
	size_t	i;
	size_t	word_len;

	res = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (*s)
	{
		word_len = 0;
		start = (char *)s;
		move_ptr_by_one_word(&s, &word_len, c);
		if (word_len == 0)
			continue ;
		res[i] = ft_strndup(start, word_len);
		if (!handle_mem_alloc_failure(&res, i))
			return (NULL);
		i++;
	}
	res[i] = NULL;
	return (res);
}
