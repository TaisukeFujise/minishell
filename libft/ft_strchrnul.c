#include "libft.h"

char	*ft_strchrnul(const char *s, int c)
{
	while (*s && (unsigned char)*s != (unsigned char)c)
		s++;
	return ((char *)s);
}
