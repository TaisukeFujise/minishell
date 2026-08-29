/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   name.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 19:35:28 by fendo             #+#    #+#             */
/*   Updated: 2026/08/25 19:35:32 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

size_t	str_name_len(const char *str)
{
	size_t	len;

	if (!str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	len = 1;
	while (ft_isalnum(str[len]) || str[len] == '_')
		len++;
	return (len);
}

size_t	str_assign_pos(const char *str, bool *append)
{
	size_t	name_len;

	if (append)
		*append = false;
	name_len = str_name_len(str);
	if (name_len == 0)
		return (0);
	if (str[name_len] == '=')
		return (name_len);
	if (str[name_len] == '+' && str[name_len + 1] == '=')
	{
		if (append)
			*append = true;
		return (name_len + 1);
	}
	return (0);
}
