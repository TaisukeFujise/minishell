/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arena_destroy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 20:13:33 by fendo             #+#    #+#             */
/*   Updated: 2026/08/27 20:13:33 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_arena_destroy(t_arena *arena)
{
	t_arena_chunk	*chunk;
	t_arena_chunk	*prev;

	if (!arena)
		return ;
	chunk = arena->head;
	while (chunk)
	{
		prev = chunk->prev;
		free(chunk);
		chunk = prev;
	}
	arena->head = NULL;
}
