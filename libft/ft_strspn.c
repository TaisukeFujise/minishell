/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strspn.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 20:14:43 by fendo             #+#    #+#             */
/*   Updated: 2026/08/27 20:14:44 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strspn(char **line, const char *charset, bool is_include)
{
	size_t	len;

	len = 0;
	if (is_include)
	{
		while ((*line)[len] != '\0' && ft_strchr(charset, (*line)[len]))
			len++;
	}
	else
	{
		while ((*line)[len] != '\0' && !ft_strchr(charset, (*line)[len]))
			len++;
	}
	*line += len;
	return (len);
}
