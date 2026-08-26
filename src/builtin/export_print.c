/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 19:38:15 by fendo             #+#    #+#             */
/*   Updated: 2026/08/25 19:38:16 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"

#define EXPORT_CTRL "\033\a\b\t\n\v\f\r"
#define EXPORT_ESC "Eabtnvfr"

/*
	Whether the character can stand as it is. Inside double quotes only
	the four the shell would read again have to be escaped; inside single
	quotes everything unprintable does, and the quote and the backslash.
*/
static bool	is_plain(char c, char quote)
{
	if (quote == '"')
		return (ft_strchr("\"\\$`", c) == NULL);
	return (ft_isprint((unsigned char)c) && c != '\\' && c != '\'');
}

/*
	The escape of one character: the letter bash gives a control
	character, its octal code when it has no letter, and the character
	itself behind a backslash otherwise.
*/
static bool	put_escape(char c, char quote)
{
	char	out[4];
	char	*esc;

	out[0] = '\\';
	out[1] = c;
	esc = ft_strchr(EXPORT_CTRL, c);
	if (quote == '\'' && esc != NULL)
		out[1] = EXPORT_ESC[esc - EXPORT_CTRL];
	else if (quote == '\'' && !ft_isprint((unsigned char)c))
	{
		out[1] = '0' + ((unsigned char)c >> 6 & 3);
		out[2] = '0' + ((unsigned char)c >> 3 & 7);
		out[3] = '0' + ((unsigned char)c & 7);
		return (write_all(STDOUT_FILENO, out, 4));
	}
	return (write_all(STDOUT_FILENO, out, 2));
}

/*
	The value between its quotes. What needs no escape is written in one
	go, so a long value costs one write and not one per character.
*/
static bool	put_value(char *str, char quote)
{
	char	*run;

	if (!write_all(STDOUT_FILENO, &quote, 1))
		return (false);
	while (*str)
	{
		run = str;
		while (*str && is_plain(*str, quote))
			str++;
		if (!write_all(STDOUT_FILENO, run, (size_t)(str - run)))
			return (false);
		if (*str && !put_escape(*str++, quote))
			return (false);
	}
	return (write_all(STDOUT_FILENO, &quote, 1));
}

/*
	"declare -x NAME", then the value when the name has one. A value
	that is not printable throughout is shown in the $'...' form.
	Returns the value hash_walk reads: below zero stops the walk.
*/
int	print_export(t_bucket_contents *item)
{
	char	*scan;
	bool	ok;

	if (!item->data.exported)
		return (0);
	ok = write_all(STDOUT_FILENO, "declare -x ", 11)
		&& write_all(STDOUT_FILENO, item->key, ft_strlen(item->key));
	scan = item->data.value;
	while (scan != NULL && *scan && ft_isprint((unsigned char)*scan))
		scan++;
	if (ok && scan != NULL && *scan)
		ok = write_all(STDOUT_FILENO, "=$", 2)
			&& put_value(item->data.value, '\'');
	else if (ok && scan != NULL)
		ok = write_all(STDOUT_FILENO, "=", 1)
			&& put_value(item->data.value, '"');
	if (!ok || !write_all(STDOUT_FILENO, "\n", 1))
		return (-1);
	return (0);
}
