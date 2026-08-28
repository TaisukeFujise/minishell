/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 19:38:15 by fendo             #+#    #+#             */
/*   Updated: 2026/08/28 20:10:00 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"

/*
	The value between double quotes, escaping the four characters the
	shell would otherwise read again. Every other byte stands as it is,
	which is what keeps a value the terminal shows as text -- a name in
	Japanese, an emoji -- readable here too: bash writes those bytes
	through as well. What needs no escape is written in one go, so a long
	value costs one write and not one per character.
	[bash-5.1 builtins/setattr.def, lib/sh/shquote.c sh_double_quote()]
*/
static bool	put_value(char *str)
{
	char	*run;

	if (!write_all(STDOUT_FILENO, "\"", 1))
		return (false);
	while (*str)
	{
		run = str;
		while (*str && ft_strchr("\"\\$`", *str) == NULL)
			str++;
		if (!write_all(STDOUT_FILENO, run, (size_t)(str - run)))
			return (false);
		if (*str && !write_all(STDOUT_FILENO, "\\", 1))
			return (false);
		if (*str && !write_all(STDOUT_FILENO, str++, 1))
			return (false);
	}
	return (write_all(STDOUT_FILENO, "\"", 1));
}

/*
	"declare -x NAME", then the value when the name has one.
	Returns the value hash_walk reads: below zero stops the walk.
*/
int	print_export(t_bucket_contents *item)
{
	bool	ok;

	if (!item->data.exported)
		return (0);
	ok = write_all(STDOUT_FILENO, "declare -x ", 11)
		&& write_all(STDOUT_FILENO, item->key, ft_strlen(item->key));
	if (ok && item->data.value != NULL)
		ok = write_all(STDOUT_FILENO, "=", 1)
			&& put_value(item->data.value);
	if (!ok || !write_all(STDOUT_FILENO, "\n", 1))
		return (-1);
	return (0);
}
