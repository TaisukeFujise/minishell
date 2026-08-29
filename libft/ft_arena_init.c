/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arena_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 20:13:35 by fendo             #+#    #+#             */
/*   Updated: 2026/08/27 20:13:36 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_arena_init(t_arena *arena, size_t default_cap)
{
	if (!arena)
		return ;
	arena->head = NULL;
	arena->default_cap = default_cap;
	if (arena->default_cap == 0)
		arena->default_cap = ARENA_DEFAULT_CHUNK_SIZE;
}
