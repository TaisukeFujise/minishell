#include "../../include/builtin.h"
#include "../../include/strbuf.h"

#define EXPORT_CTRL "\033\a\b\t\n\v\f\r"
#define EXPORT_ESC "Eabtnvfr"

/*
	One character of a value, as export has to show it back. A control
	character becomes the escape bash writes for it, anything else that
	is not printable becomes its octal code.
*/
static bool	append_escaped(t_strbuf *buf, char c)
{
	char	*esc;
	char	oct[4];

	esc = ft_strchr(EXPORT_CTRL, c);
	if (esc != NULL && c != '\0')
	{
		oct[0] = '\\';
		oct[1] = EXPORT_ESC[esc - EXPORT_CTRL];
		return (strbuf_append(buf, oct, 2));
	}
	if (!ft_isprint((unsigned char)c))
	{
		oct[0] = '\\';
		oct[1] = '0' + ((unsigned char)c >> 6 & 3);
		oct[2] = '0' + ((unsigned char)c >> 3 & 7);
		oct[3] = '0' + ((unsigned char)c & 7);
		return (strbuf_append(buf, oct, 4));
	}
	if (c == '\\' || c == '\'')
		return (strbuf_append(buf, "\\", 1) && strbuf_append(buf, &c, 1));
	return (strbuf_append(buf, &c, 1));
}

/*
	The value between its quotes. Inside single quotes the value is shown
	as $'...', where every character has an escape; inside double quotes
	only the four characters the shell would still read are escaped.
*/
static bool	append_quoted(t_strbuf *buf, char *str, char quote)
{
	if (!strbuf_append(buf, &quote, 1))
		return (false);
	while (*str)
	{
		if (quote == '\'')
		{
			if (!append_escaped(buf, *str))
				return (false);
		}
		else if (ft_strchr("\"\\$`", *str) != NULL)
		{
			if (!strbuf_append(buf, "\\", 1)
				|| !strbuf_append(buf, str, 1))
				return (false);
		}
		else if (!strbuf_append(buf, str, 1))
			return (false);
		str++;
	}
	return (strbuf_append(buf, &quote, 1));
}

/*
	"declare -x NAME", then the value when the name has one. A value that
	is not printable throughout is shown in the $'...' form.
*/
static bool	build_export_line(t_strbuf *buf, t_bucket_contents *item)
{
	char	*scan;

	if (!strbuf_append(buf, "declare -x ", 11)
		|| !strbuf_append(buf, item->key, ft_strlen(item->key)))
		return (false);
	scan = item->data.value;
	while (scan != NULL && *scan && ft_isprint((unsigned char)*scan))
		scan++;
	if (scan != NULL && *scan)
	{
		if (!strbuf_append(buf, "=$", 2))
			return (false);
		if (!append_quoted(buf, item->data.value, '\''))
			return (false);
	}
	else if (scan != NULL)
	{
		if (!strbuf_append(buf, "=", 1)
			|| !append_quoted(buf, item->data.value, '"'))
			return (false);
	}
	return (strbuf_append(buf, "\n", 1));
}

/*
	The line is built whole and written once: a value can be of any
	length, and the output of the shell must not be cut in the middle by
	another write. [review D37-23]
	Returns the value hash_walk reads: below zero stops the walk.
*/
int	print_export(t_bucket_contents *item)
{
	t_strbuf	buf;
	bool		ok;

	if (!item->data.exported)
		return (0);
	if (!strbuf_init(&buf))
		return (-1);
	ok = build_export_line(&buf, item);
	if (ok)
		ok = write_all(STDOUT_FILENO, buf.data, buf.len);
	strbuf_free(&buf);
	if (!ok)
		return (-1);
	return (0);
}
