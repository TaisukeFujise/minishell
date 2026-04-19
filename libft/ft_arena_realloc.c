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

static int	grow_in_place(t_arena *arena, void *ptr,
				size_t old_size, size_t new_size)
{
	size_t	old_aligned;
	size_t	new_aligned;
	size_t	offset;

	if (!arena->current || align_size(old_size, &old_aligned))
		return (0);
	if (align_size(new_size, &new_aligned))
		return (0);
	if ((char *)ptr < arena->current->data
		|| (char *)ptr >= arena->current->data + arena->current->capacity)
		return (0);
	offset = (size_t)((char *)ptr - arena->current->data);
	if (offset + old_aligned != arena->current->used)
		return (0);
	if (new_aligned < old_aligned)
		arena->current->used -= old_aligned - new_aligned;
	else if (new_aligned > old_aligned)
	{
		if (new_aligned - old_aligned > arena->current->capacity
			- arena->current->used)
			return (0);
		arena->current->used += new_aligned - old_aligned;
	}
	return (1);
}

void	*ft_arena_realloc(t_arena *arena, void *ptr,
			size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_len;

	if (!arena)
		return (NULL);
	if (!ptr)
		return (ft_arena_alloc(arena, new_size));
	if (grow_in_place(arena, ptr, old_size, new_size))
		return (ptr);
	new_ptr = ft_arena_alloc(arena, new_size);
	if (!new_ptr)
		return (NULL);
	copy_len = old_size;
	if (new_size < copy_len)
		copy_len = new_size;
	if (copy_len > 0)
		ft_memcpy(new_ptr, ptr, copy_len);
	return (new_ptr);
}

// int	main(void)
// {
// 	t_arena	arena;
// 	char	*buf;
// 	char	*grown;
// 	char	*shrunk;

// 	ft_arena_init(&arena, 128);
// 	buf = ft_arena_alloc(&arena, 4);
// 	if (!buf)
// 		return (1);
// 	buf[0] = 'a';
// 	buf[1] = 'b';
// 	buf[2] = 'c';
// 	buf[3] = '\0';
// 	grown = ft_arena_realloc(&arena, buf, 4, 12);
// 	if (!grown || grown[0] != 'a' || grown[1] != 'b' || grown[2] != 'c')
// 	{
// 		ft_arena_destroy(&arena);
// 		return (1);
// 	}
// 	shrunk = ft_arena_realloc(&arena, grown, 12, 2);
// 	if (!shrunk || shrunk[0] != 'a' || shrunk[1] != 'b')
// 	{
// 		ft_arena_destroy(&arena);
// 		return (1);
// 	}
// 	ft_arena_destroy(&arena);
// 	return (0);
// }
