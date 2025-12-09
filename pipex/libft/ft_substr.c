/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 09:27:55 by tafujise          #+#    #+#             */
/*   Updated: 2025/11/09 11:23:27 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*substr;
	unsigned int	s_len;
	size_t			i;

	s_len = ft_strlen(s);
	if (start + 1 > s_len || (int)start < 0)
	{
		substr = malloc(1);
		*substr = '\0';
		return (substr);
	}
	if (s_len - start >= len)
		substr = malloc(sizeof(char) * (len + 1));
	else
		substr = malloc(sizeof(char) * (s_len - start + 1));
	if (!substr)
		return (NULL);
	i = 0;
	while (s[i + start] && len-- > 0)
	{
		substr[i] = s[i + start];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}
