#include "libft.h"

void	ft_arena_init(t_arena *arena, size_t default_cap)
{
	if (!arena)
		return ;
	arena->current = NULL;
	arena->default_cap = default_cap;
	if (arena->default_cap == 0)
		arena->default_cap = ARENA_DEFAULT_CHUNK_SIZE;
}
