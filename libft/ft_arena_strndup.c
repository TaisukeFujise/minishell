#include "libft.h"

char	*ft_arena_strndup(t_arena *arena, const char *src, size_t n)
{
	char	*dst;

	if (!arena || !src || n == SIZE_MAX)
		return (NULL);
	dst = ft_arena_alloc(arena, n + 1);
	if (!dst)
		return (NULL);
	ft_memcpy(dst, src, n);
	dst[n] = '\0';
	return (dst);
}

// int	main(void)
// {
// 	t_arena	arena;
// 	char	*copy;
// 	char	*empty;

// 	ft_arena_init(&arena, 128);
// 	copy = ft_arena_strndup(&arena, "hello", 3);
// 	empty = ft_arena_strndup(&arena, "hello", 0);
// 	if (!copy || !empty || ft_strcmp(copy, "hel") != 0
// 		|| ft_strcmp(empty, "") != 0)
// 	{
// 		ft_arena_destroy(&arena);
// 		return (1);
// 	}
// 	ft_arena_destroy(&arena);
// 	return (0);
// }
