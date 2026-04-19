#include "libft.h"

static int	check_add_overflow(size_t a, size_t b, size_t *result)
{
	if (a > SIZE_MAX - b)
		return (1);
	*result = a + b;
	return (0);
}

char	*ft_arena_strdup(t_arena *arena, const char *src)
{
	char	*dst;
	size_t	len;
	size_t	total;

	if (!arena || !src)
		return (NULL);
	len = ft_strlen(src);
	if (check_add_overflow(len, 1, &total))
		return (NULL);
	dst = ft_arena_alloc(arena, total);
	if (!dst)
		return (NULL);
	ft_memcpy(dst, src, total);
	return (dst);
}

// int	main(void)
// {
// 	t_arena	arena;
// 	char	*copy;

// 	ft_arena_init(&arena, 128);
// 	copy = ft_arena_strdup(&arena, "hello");
// 	if (!copy || ft_strcmp(copy, "hello") != 0)
// 	{
// 		ft_arena_destroy(&arena);
// 		return (1);
// 	}
// 	ft_arena_destroy(&arena);
// 	return (0);
// }
