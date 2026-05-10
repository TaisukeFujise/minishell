#include "libft.h"

static void	free_strs(char *s1, char *s2, int flags)
{
	if ((flags & (1 << 0)) && s1)
		free(s1);
	if ((flags & (1 << 1)) && s2)
		free(s2);
}

char	*ft_strjoin_free(char *s1, char *s2, int flags)
{
	char	*ans;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
		return (free_strs(s1, s2, flags), NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	ans = malloc(len1 + len2 + 1);
	if (!ans)
		return (free_strs(s1, s2, flags), NULL);
	ft_memcpy(ans, s1, len1);
	ft_memcpy(ans + len1, s2, len2 + 1);
	free_strs(s1, s2, flags);
	return (ans);
}
