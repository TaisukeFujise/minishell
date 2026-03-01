#include "libft.h"

static int	check_mul_overflow(size_t a, size_t b, size_t *result)
{
	if (a != 0 && b > SIZE_MAX / a)
		return (1);
	*result = a * b;
	return (0);
}

void	*ft_arena_calloc(t_arena *arena, size_t count, size_t size)
{
	void	*ptr;
	size_t	total;

	if (!arena || check_mul_overflow(count, size, &total))
		return (NULL);
	ptr = ft_arena_alloc(arena, total);
	if (ptr)
		ft_memset(ptr, 0, total);
	return (ptr);
}
