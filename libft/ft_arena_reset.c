#include "libft.h"

void	ft_arena_reset(t_arena *arena)
{
	t_arena_chunk	*chunk;

	if (!arena)
		return ;
	chunk = arena->current;
	while (chunk)
	{
		chunk->used = 0;
		chunk = chunk->prev;
	}
}
