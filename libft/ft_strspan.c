#include "libft.h"

size_t	ft_strspan(char **line, const char *charset, bool is_include)
{
	size_t	len;

	len = 0;
	if (is_include)
	{
		while ((*line)[len] != '\0' && ft_strchr(charset, (*line)[len]))
			len++;
	}
	else
	{
		while ((*line)[len] != '\0' && !ft_strchr(charset, (*line)[len]))
			len++;
	}
	*line += len;
	return (len);
}
