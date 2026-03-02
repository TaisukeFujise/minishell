#include "libft.h"

static int	check_add_overflow(size_t a, size_t b, size_t *result)
{
	if (a > SIZE_MAX - b)
		return (1);
	*result = a + b;
	return (0);
}

static int	align_size(size_t size, size_t *aligned)
{
	size_t	align;
	size_t	tmp;

	align = sizeof(max_align_t);
	if (size == 0)
		size = 1;
	if (check_add_overflow(size, align - 1, &tmp))
		return (1);
	*aligned = (tmp / align) * align;
	return (0);
}

static t_arena_chunk	*new_chunk(t_arena *arena, size_t min_data_size)
{
	t_arena_chunk	*chunk;
	size_t			cap;
	size_t			total_bytes;

	cap = arena->default_cap;
	if (min_data_size > cap)
		cap = min_data_size;
	if (check_add_overflow(sizeof(t_arena_chunk), cap, &total_bytes))
		return (NULL);
	chunk = malloc(total_bytes);
	if (!chunk)
		return (NULL);
	chunk->prev = arena->current;
	chunk->capacity = cap;
	chunk->used = 0;
	arena->current = chunk;
	return (chunk);
}

static t_arena_chunk	*find_chunk(t_arena *arena, size_t aligned)
{
	t_arena_chunk	*chunk;

	chunk = arena->current;
	while (chunk)
	{
		if (aligned <= chunk->capacity
			&& chunk->used <= chunk->capacity - aligned)
			return (chunk);
		chunk = chunk->prev;
	}
	return (NULL);
}

void	*ft_arena_alloc(t_arena *arena, size_t size)
{
	t_arena_chunk	*chunk;
	size_t			aligned;
	void			*ptr;

	if (!arena || align_size(size, &aligned))
		return (NULL);
	chunk = find_chunk(arena, aligned);
	if (!chunk)
	{
		chunk = new_chunk(arena, aligned);
		if (!chunk)
			return (NULL);
	}
	arena->current = chunk;
	ptr = chunk->data + chunk->used;
	chunk->used += aligned;
	return (ptr);
}

// int	main(void)
// {
// 	t_arena	arena;
// 	void	*a;
// 	void	*b;

// 	ft_arena_init(&arena, 128);
// 	a = ft_arena_alloc(&arena, 8);
// 	b = ft_arena_alloc(&arena, 16);
// 	if (!a || !b)
// 	{
// 		ft_arena_destroy(&arena);
// 		return (1);
// 	}
// 	ft_arena_destroy(&arena);
// 	return (0);
// }
