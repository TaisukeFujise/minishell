#include "expand_internal.h"

bool	pattern_has_glob(const char *s)
{
	while (*s)
	{
		if (*s == '*')
			return (true);
		s++;
	}
	return (false);
}

static bool	fnmatch_simple(const char *pattern, const char *str)
{
	while (*pattern && *str)
	{
		if (*pattern == '*')
		{
			pattern++;
			if (!*pattern)
				return (true);
			while (*str)
			{
				if (fnmatch_simple(pattern, str))
					return (true);
				str++;
			}
			return (false);
		}
		if (*pattern != *str)
			return (false);
		pattern++;
		str++;
	}
	while (*pattern == '*')
		pattern++;
	return (*pattern == '\0' && *str == '\0');
}

static void	sort_strings(char **arr, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static int	collect_matches(t_expand_ctx *ex, const char *pattern,
							char ***out_arr)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**arr;
	int				count;
	int				cap;

	dir = opendir(".");
	if (!dir)
		return (0);
	cap = 16;
	arr = ft_arena_alloc(ex->tmp, sizeof(char *) * cap);
	count = 0;
	entry = readdir(dir);
	while (entry && arr)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
			;
		else if (fnmatch_simple(pattern, entry->d_name))
		{
			if (count >= cap)
			{
				arr = ft_arena_realloc(ex->tmp, arr,
						sizeof(char *) * cap, sizeof(char *) * cap * 2);
				cap *= 2;
			}
			if (arr)
				arr[count++] = ft_arena_strdup(ex->tmp, entry->d_name);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	*out_arr = arr;
	return (count);
}

/*
** Expand glob pattern to matching filenames.
** Returns array of matched filenames (sorted alphabetically).
** If no matches, returns array with the original pattern.
** All strings allocated in ex->ast.
*/
char	**expand_glob(t_expand_ctx *ex, const char *pattern, int *count)
{
	char	**matches;
	char	**result;
	int		nmatches;
	int		i;

	if (!pattern_has_glob(pattern))
	{
		result = ft_arena_alloc(ex->ast, sizeof(char *));
		if (!result)
			return (NULL);
		result[0] = ft_arena_strdup(ex->ast, pattern);
		*count = 1;
		return (result);
	}
	nmatches = collect_matches(ex, pattern, &matches);
	if (nmatches == 0)
	{
		result = ft_arena_alloc(ex->ast, sizeof(char *));
		if (!result)
			return (NULL);
		result[0] = ft_arena_strdup(ex->ast, pattern);
		*count = 1;
		return (result);
	}
	sort_strings(matches, nmatches);
	result = ft_arena_alloc(ex->ast, sizeof(char *) * nmatches);
	if (!result)
		return (NULL);
	i = -1;
	while (++i < nmatches)
		result[i] = ft_arena_strdup(ex->ast, matches[i]);
	*count = nmatches;
	return (result);
}
