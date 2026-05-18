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

// int	main(void)
// {
// 	t_arena	arena;
// 	int		*arr;
// 	size_t	i;

// 	ft_arena_init(&arena, 128);
// 	arr = ft_arena_calloc(&arena, 4, sizeof(int));
// 	if (!arr)
// 	{
// 		ft_arena_destroy(&arena);
// 		return (1);
// 	}
// 	i = 0;
// 	while (i < 4)
// 	{
// 		if (arr[i] != 0)
// 		{
// 			ft_arena_destroy(&arena);
// 			return (1);
// 		}
// 		i++;
// 	}
// 	ft_arena_destroy(&arena);
// 	return (0);
// }
