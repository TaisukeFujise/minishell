/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 10:09:58 by tafujise          #+#    #+#             */
/*   Updated: 2025/11/05 23:49:02 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;
	size_t	total_len;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	total_len = ft_strlen(s1) + ft_strlen(s2);
	res = malloc(sizeof(char) * (total_len + 1));
	if (!res)
		return (NULL);
	while (s1 && *s1)
	{
		*res = *s1;
		res++;
		s1++;
	}
	while (s2 && *s2)
	{
		*res = *s2;
		res++;
		s2++;
	}
	*res = '\0';
	return (res - total_len);
}
