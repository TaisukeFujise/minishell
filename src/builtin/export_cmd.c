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
	The name that comes after the one just printed, or NULL when there is
	none left. export lists its names in order; walking the table again
	for each name costs nothing at this size and needs no array to sort.
*/
static t_bucket_contents	*next_in_order(t_hashtable *table, char *prev)
{
	t_bucket_contents	*item;
	t_bucket_contents	*best;
	int					i;

	best = NULL;
	i = 0;
	while (i < table->bucket_size)
	{
		item = hash_items(i, table);
		while (item != NULL)
		{
			if (item->data.exported
				&& (prev == NULL || ft_strcmp(item->key, prev) > 0)
				&& (best == NULL || ft_strcmp(item->key, best->key) < 0))
				best = item;
			item = item->next;
		}
		i++;
	}
	return (best);
}

static t_status	print_exported(t_hashtable *table)
{
	t_bucket_contents	*item;

	if (table == NULL)
		return (ST_OK);
	item = next_in_order(table, NULL);
	while (item != NULL)
	{
		if (print_export(item) < 0)
			return (ST_FAILURE);
		item = next_in_order(table, item->key);
	}
	return (ST_OK);
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

	if (!args)
		return (print_exported(ctx->env_table));
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
