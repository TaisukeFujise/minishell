#include "libft.h"
#include <stdio.h>

static int	test_chunk_chain(void)
{
	t_arena			arena;
	t_arena_chunk	*head;
	t_arena_chunk	*older;
	size_t			unit;
	int				ok;

	unit = sizeof(max_align_t);
	ft_arena_init(&arena, unit * 4);
	if (!ft_arena_alloc(&arena, unit * 3)
		|| !ft_arena_alloc(&arena, unit * 5))
	{
		ft_arena_destroy(&arena);
		return (0);
	}
	head = arena.head;
	older = head->prev;
	ok = (older != NULL && ft_arena_alloc(&arena, unit) != NULL);
	ok = ok && arena.head == head && head->prev == older;
	ft_arena_reset(&arena);
	ok = ok && head->used == 0 && older->used == 0;
	ft_arena_destroy(&arena);
	return (ok && arena.head == NULL);
}

static int	test_alignment(void)
{
	t_arena	arena;
	void	*first;
	void	*second;
	int		ok;

	ft_arena_init(&arena, 128);
	first = ft_arena_alloc(&arena, 1);
	second = ft_arena_alloc(&arena, sizeof(long double));
	ok = first != NULL && second != NULL;
	ok = ok && (uintptr_t)first % _Alignof(max_align_t) == 0;
	ok = ok && (uintptr_t)second % _Alignof(max_align_t) == 0;
	ft_arena_destroy(&arena);
	return (ok);
}

int	main(void)
{
	int	passed;

	passed = test_chunk_chain() + test_alignment();
	printf("arena tests: %d/2 passed\n", passed);
	return (passed != 2);
}
