/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_glob.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 19:36:55 by fendo             #+#    #+#             */
/*   Updated: 2026/08/25 19:36:56 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

static bool	star_rewind(const char **pat, const char **name,
					const char *star, const char **mark)
{
	if (!star)
		return (false);
	(*mark)++;
	*pat = star + 1;
	*name = *mark;
	return (true);
}

static bool	glob_match(const char *pat, const char *mask, const char *name)
{
	const char	*base;
	const char	*star;
	const char	*mark;

	base = pat;
	star = NULL;
	mark = NULL;
	while (*name)
	{
		if (*pat == '*' && mask[pat - base])
		{
			star = pat++;
			mark = name;
		}
		else if (*pat == *name)
		{
			pat++;
			name++;
		}
		else if (!star_rewind(&pat, &name, star, &mark))
			return (false);
	}
	while (*pat == '*' && mask[pat - base])
		pat++;
	return (*pat == '\0');
}

static bool	glob_insert(t_expand *exp, t_word_list **list, const char *name)
{
	while (*list && ft_strcmp((*list)->wd->str, name) < 0)
		list = &(*list)->next;
	return (field_insert(exp, list, name, ft_strlen(name)) != NULL);
}

/*
	A leading dot has to be matched by a literal dot in the pattern, and even
	then "." and ".." are never produced.
*/
static bool	glob_keep(const char *pat, const char *mask, const char *name)
{
	if (name[0] == '.')
	{
		if (pat[0] != '.')
			return (false);
		if (name[1] == '\0' || (name[1] == '.' && name[2] == '\0'))
			return (false);
	}
	return (glob_match(pat, mask, name));
}

t_word_list	*expand_glob(t_expand *exp)
{
	DIR				*dir;
	struct dirent	*ent;
	t_word_list		*list;
	const char		*pat;

	list = NULL;
	pat = exp->buf.text.data;
	dir = opendir(".");
	if (!dir)
		return (field_insert(exp, &list, pat, exp->buf.text.len));
	ent = readdir(dir);
	while (ent)
	{
		if (glob_keep(pat, exp->buf.mask.data, ent->d_name)
			&& !glob_insert(exp, &list, ent->d_name))
		{
			closedir(dir);
			return (NULL);
		}
		ent = readdir(dir);
	}
	closedir(dir);
	if (!list)
		return (field_insert(exp, &list, pat, exp->buf.text.len));
	return (list);
}
