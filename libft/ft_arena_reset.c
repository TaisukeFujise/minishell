/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arena_reset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 20:13:38 by fendo             #+#    #+#             */
/*   Updated: 2026/08/27 20:13:39 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_arena_reset(t_arena *arena)
{
	t_arena_chunk	*chunk;

	if (!arena)
		return ;
	chunk = arena->head;
	while (chunk)
	{
		chunk->used = 0;
		chunk = chunk->prev;
	}
}
