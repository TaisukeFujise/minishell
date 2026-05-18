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

static bool	insert_match(t_expand *ex, t_word_list **list, const char *name)
{
	t_fields	fields;
	t_word_list	*node;

	fields.head = NULL;
	fields.tail = &fields.head;
	node = fields_add(ex, &fields, name, ft_strlen(name));
	if (!node)
		return (false);
	while (*list && ft_strcmp((*list)->wd->str, node->wd->str) < 0)
		list = &(*list)->next;
	node->next = *list;
	*list = node;
	return (true);
}

static t_word_list	*append_matches(t_fields *fields, t_word_list *list)
{
	*fields->tail = list;
	while (*fields->tail)
		fields->tail = &(*fields->tail)->next;
	return (list);
}

t_word_list	*append_glob(t_expand *ex, t_fields *fields, const char *pat)
{
	DIR				*dir;
	struct dirent	*ent;
	t_word_list		*list;

	dir = opendir(".");
	if (!dir)
		return (fields_add(ex, fields, pat, ft_strlen(pat)));
	list = NULL;
	ent = readdir(dir);
	while (ent)
	{
		if ((pat[0] == '.' || ent->d_name[0] != '.')
			&& match_star(pat, ent->d_name)
			&& !insert_match(ex, &list, ent->d_name))
		{
			closedir(dir);
			return (NULL);
		}
		ent = readdir(dir);
	}
	closedir(dir);
	if (!list)
		return (fields_add(ex, fields, pat, ft_strlen(pat)));
	return (append_matches(fields, list));
}
