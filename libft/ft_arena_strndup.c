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
