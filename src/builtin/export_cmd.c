/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:40:50 by tafujise          #+#    #+#             */
/*   Updated: 2026/05/10 17:14:08 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"

#define EXPORT_CTRL "\033\a\b\t\n\v\f\r"
#define EXPORT_ESC "Eabtnvfr"

static void	print_escaped_value(char *s, bool use_ansic_quote)
{
	char	*esc;

	while (*s)
	{
		if (!use_ansic_quote && ft_strchr("\"\\$`", *s))
			printf("\\%c", *s);
		else if (!use_ansic_quote)
			printf("%c", *s);
		else
		{
			esc = ft_strchr(EXPORT_CTRL, *s);
			if (esc)
				printf("\\%c", EXPORT_ESC[esc - EXPORT_CTRL]);
			else if (!ft_isprint((unsigned char)*s))
				printf("\\%03o", (unsigned char)*s);
			else if (*s == '\\' || *s == '\'')
				printf("\\%c", *s);
			else
				printf("%c", *s);
		}
		s++;
	}
}

static int	print_export(t_bucket_contents *item)
{
	char	*p;

	if (!item->data.exported)
		return (0);
	printf("declare -x %s", item->key);
	if (item->data.value)
	{
		p = item->data.value;
		while (*p && ft_isprint((unsigned char)*p))
			p++;
		if (*p)
		{
			printf("=$'");
			print_escaped_value(item->data.value, true);
			printf("'");
		}
		else
		{
			printf("=\"");
			print_escaped_value(item->data.value, false);
			printf("\"");
		}
	}
	printf("\n");
	return (0);
}

static t_status	set_export_value(t_word *wd, t_bucket_contents *item)
{
	char	*value;

	value = ft_strndup(wd->eq_ptr + 1,
			wd->len - (int)(wd->eq_ptr + 1 - wd->str));
	if (!value)
		return (ST_FATAL);
	if ((wd->flag & W_APPEND) && item->data.value)
	{
		value = ft_strjoin_free(item->data.value, value, 1 << 1);
		if (!value)
			return (ST_FATAL);
	}
	free(item->data.value);
	item->data.value = value;
	return (ST_OK);
}

static t_status	put_export(t_word *wd, t_ctx *ctx)
{
	t_bucket_contents	*item;
	char				*key;
	int					len;

	len = wd->len;
	if (wd->flag & (W_ASSIGN | W_APPEND))
		len = wd->eq_ptr - wd->str - ((wd->flag & W_APPEND) != 0);
	key = ft_strndup(wd->str, len);
	if (!key)
		return (ST_FATAL);
	item = hash_insert(key, ctx->env_table);
	if (!item)
	{
		free(key);
		return (ST_FATAL);
	}
	if (item->key != key)
		free(key);
	item->data.exported = true;
	if (wd->flag & (W_ASSIGN | W_APPEND))
		return (set_export_value(wd, item));
	return (ST_OK);
}

/*
	export name[=word]...
	"export" register the args to env table, and update the ctx->exit_code.
	If no parameters, it displays exported vars.
	(i) no parameters
	(ii) only name
	(iii) key=value
*/
t_status	export_cmd(t_word_list *args, t_ctx *ctx)
{
	t_status	status;

	if (!args)
	{
		hash_walk(ctx->env_table, print_export);
		return (ST_OK);
	}
	status = ST_OK;
	while (args)
	{
		if (!(args->wd->flag & (W_ID | W_ASSIGN | W_APPEND)))
		{
			ft_putendl_fd("minishell: export: not a valid identifier",
				STDERR_FILENO);
			status = ST_FAILURE;
		}
		else if (put_export(args->wd, ctx) == ST_FATAL)
			return (ST_FATAL);
		args = args->next;
	}
	return (status);
}
