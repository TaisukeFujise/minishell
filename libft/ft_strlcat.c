/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 22:04:49 by fujisetaisu       #+#    #+#             */
/*   Updated: 2025/10/17 22:08:09 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dsize)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (dsize <= dst_len)
		return (dsize + src_len);
	while (*dst)
		dst++;
	i = 0;
	while (src[i] && i < (dsize - dst_len - 1))
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst_len + src_len);
}
