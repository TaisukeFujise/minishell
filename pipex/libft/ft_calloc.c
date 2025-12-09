/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 09:02:39 by tafujise          #+#    #+#             */
/*   Updated: 2025/10/17 22:06:41 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	total;

	if (size != 0 && nmemb >= SIZE_MAX / size)
		return (NULL);
	total = nmemb * size;
	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	ptr = ft_memset(ptr, 0, total);
	return (ptr);
}
