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
