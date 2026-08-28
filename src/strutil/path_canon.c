/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_canon.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 22:10:00 by fendo             #+#    #+#             */
/*   Updated: 2026/08/28 22:10:00 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	base + "/" + arg, or a copy of arg when it already is a path from
	the root. NULL when there is no base to start from, which only
	happens before the shell has learned where it is.
*/
char	*path_absolute(char *base, char *arg)
{
	size_t	len;
	size_t	arg_len;
	char	*full;

	if (*arg == '/')
		return (ft_strdup(arg));
	if (base == NULL)
		return (NULL);
	len = ft_strlen(base);
	while (len > 0 && base[len - 1] == '/')
		len--;
	arg_len = ft_strlen(arg);
	full = malloc(len + arg_len + 2);
	if (full == NULL)
		return (NULL);
	ft_memcpy(full, base, len);
	full[len] = '/';
	ft_memcpy(full + len + 1, arg, arg_len + 1);
	return (full);
}

/*
	One step back. Q ends just past the component being dropped, so it
	walks back over that component and then over the separator, and
	stops at the root, where ".." is the root itself.
*/
static char	*canon_pop(char *base, char *q)
{
	while (q > base && q[-1] != '/')
		q--;
	if (q > base)
		q--;
	return (q);
}

/*
	The next component, written where the last one ended.
*/
static char	*canon_push(char *base, char *q, char **p)
{
	if (q != base)
		*q++ = '/';
	while (**p != '\0' && **p != '/')
		*q++ = *(*p)++;
	return (q);
}

/*
	"." and ".." resolved in the name itself, in place, never growing.
	This is what makes the shell's idea of where it is a logical one:
	"link/.." leads back to the directory the name was written from and
	not to the physical parent of the link, as bash, dash and zsh all
	do. A leading "//" is not kept apart from "/".
	[bash-5.3 lib/sh/pathcanon.c sh_canonpath()]
*/
char	*path_canon(char *path)
{
	char	*base;
	char	*p;
	char	*q;

	if (path == NULL)
		return (NULL);
	base = path + 1;
	p = base;
	q = base;
	while (*p != '\0')
	{
		if (*p == '/')
			p++;
		else if (p[0] == '.' && (p[1] == '/' || p[1] == '\0'))
			p++;
		else if (p[0] == '.' && p[1] == '.' && (p[2] == '/' || p[2] == '\0'))
		{
			p += 2;
			q = canon_pop(base, q);
		}
		else
			q = canon_push(base, q, &p);
	}
	*q = '\0';
	return (path);
}
