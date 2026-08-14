#include "expand_internal.h"

static bool	match_star(const char *pat, const char *name)
{
	if (*pat == '\0')
		return (*name == '\0');
	if (*pat != '*')
		return (*name == *pat && match_star(pat + 1, name + 1));
	while (*pat == '*')
		pat++;
	if (*pat == '\0')
		return (true);
	name = ft_strchr(name, *pat);
	while (name)
	{
		if (match_star(pat, name))
			return (true);
		name = ft_strchr(name + 1, *pat);
	}
	return (false);
}

static bool	insert_match(t_expand *exp, t_word_list **list, const char *name)
{
	t_word_list	*node;

	while (*list && ft_strcmp((*list)->wd->str, name) < 0)
		list = &(*list)->next;
	node = field_insert(exp, list, name, ft_strlen(name));
	return (node != NULL);
}

t_word_list	*expand_glob(t_expand *exp, const char *pat)
{
	DIR				*dir;
	struct dirent	*ent;
	t_word_list		*list;

	list = NULL;
	dir = opendir(".");
	if (!dir)
		return (field_insert(exp, &list, pat, ft_strlen(pat)));
	ent = readdir(dir);
	while (ent)
	{
		if ((pat[0] == '.' || ent->d_name[0] != '.')
			&& match_star(pat, ent->d_name)
			&& !insert_match(exp, &list, ent->d_name))
		{
			closedir(dir);
			return (NULL);
		}
		ent = readdir(dir);
	}
	closedir(dir);
	if (!list)
		return (field_insert(exp, &list, pat, ft_strlen(pat)));
	return (list);
}
