/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 19:58:11 by fendo             #+#    #+#             */
/*   Updated: 2025/12/21 22:07:56 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Refer to section "3.45 Blank Character (<blank>)" 
// in https://pubs.opengroup.org/onlinepubs/9799919799/
void	skip_space(char **line)
{
	while (**line != '\0' && ft_isblank(**line))
		(*line)++;
}
