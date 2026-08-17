#include "libft.h"

// This implementation cannot extend an allocation in place.
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (malloc(new_size));
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (old_size > new_size)
		old_size = new_size;
	ft_memcpy(new_ptr, ptr, old_size);
	free(ptr);
	return (new_ptr);
}
