#include "libft.h"

void	ft_arena_destroy(t_arena *arena)
{
	t_arena_chunk	*chunk;
	t_arena_chunk	*prev;

	if (!arena)
		return ;
	chunk = arena->current;
	while (chunk)
	{
		prev = chunk->prev;
		free(chunk);
		chunk = prev;
	}
	arena->current = NULL;
}
