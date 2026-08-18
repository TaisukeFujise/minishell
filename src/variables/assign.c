/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assigns.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 03:17:31 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/11 11:06:52 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/variables.h"

/*
	"key+=value" keeps the value the name has in the current environment.
	The old value is read before the entry is created,
	so that an entry created in tmp_table does not hide the persistent one.
*/
static bool	set_value(t_bucket_contents *item, char *old, char *value)
{
	char	*joined;
	bool	ok;

	if (old == NULL)
		return (hash_set_value(item, value));
	joined = ft_strjoin(old, value);
	if (joined == NULL)
		return (false);
	ok = hash_set_value(item, joined);
	free(joined);
	return (ok);
}

/*
	apply_assign stores one expanded assignment.
	- TMP is the environment of the current command, and is exported to it.
	- VARS is the persistent table, where the exported flag is left alone.
*/
t_status	apply_assign(t_assign *assign, t_hashtable *table, t_ctx *ctx,
		t_tabletype type)
{
	t_bucket_contents	*item;
	char				*value;
	char				*old;

	value = "";
	if (assign->value != NULL && assign->value->str != NULL)
		value = assign->value->str;
	old = NULL;
	if (assign->key->flag & W_APPEND)
		old = env_lookup(ctx->tmp_table, ctx->env_table, assign->key->str);
	item = hash_insert(assign->key->str, table);
	if (item == NULL)
		return (ST_FATAL);
	if (!set_value(item, old, value))
		return (ST_FATAL);
	if (type == TMP)
		item->data.exported = true;
	return (ST_OK);
}
