/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:40:50 by tafujise          #+#    #+#             */
/*   Updated: 2026/08/16 23:35:06 by fendo            ###   ########.fr       */
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

/*
	"export name" keeps the value the name has in the current command,
	so "A=one export A" leaves A set to one.
*/
static t_status	set_export_value(t_export_arg *arg, t_bucket_contents *item,
		t_ctx *ctx)
{
	t_bucket_contents	*tmp;
	char				*value;

	if (arg->value_pos == 0)
	{
		tmp = hash_search(arg->word->str, ctx->tmp_table);
		if (tmp == NULL)
			return (ST_OK);
		if (!hash_set_value(item, tmp->data.value))
			return (ST_FATAL);
		return (ST_OK);
	}
	value = ft_strdup(arg->word->str + arg->value_pos);
	if (!value)
		return (ST_FATAL);
	if (arg->append && item->data.value)
	{
		value = ft_strjoin_free(item->data.value, value, 1 << 1);
		if (!value)
			return (ST_FATAL);
	}
	if (!hash_set_value(item, value))
		return (free(value), ST_FATAL);
	free(value);
	return (ST_OK);
}

static t_status	put_export(t_word *wd, t_ctx *ctx)
{
	t_bucket_contents	*item;
	char				*key;
	t_export_arg		arg;

	arg.word = wd;
	arg.key_len = str_name_len(wd->str);
	arg.value_pos = str_assign_pos(wd->str, &arg.append);
	if (!arg.key_len || (!arg.value_pos && wd->str[arg.key_len]))
		return (ST_FAILURE);
	if (arg.value_pos)
		arg.value_pos++;
	key = ft_strndup(wd->str, arg.key_len);
	if (!key)
		return (ST_FATAL);
	item = hash_insert(key, ctx->env_table);
	free(key);
	if (!item)
		return (ST_FATAL);
	item->data.exported = true;
	return (set_export_value(&arg, item, ctx));
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
	t_status	arg_status;
	t_status	status;

	if (!args && hash_walk_ordered(ctx->env_table, print_export) < 0)
		return (ST_FAILURE);
	if (!args)
		return (ST_OK);
	status = ST_OK;
	while (args)
	{
		arg_status = put_export(args->wd, ctx);
		if (arg_status == ST_FAILURE)
		{
			print_error_name("export", args->wd->str,
				"not a valid identifier");
			status = ST_FAILURE;
		}
		else if (arg_status == ST_FATAL)
			return (ST_FATAL);
		args = args->next;
	}
	return (status);
}
