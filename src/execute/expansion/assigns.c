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

#include "../../../include/execute.h"
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

/*
	The expander owns the expanded value in the ast arena,
	so the table has to keep its own copy.
*/
static t_status	store_value(t_bucket_contents *item, t_word *value)
{
	char	*copy;

	if (value == NULL || value->str == NULL)
		copy = ft_strdup("");
	else
		copy = ft_strdup(value->str);
	if (copy == NULL)
		return (ST_FATAL);
	free(item->data.value);
	item->data.value = copy;
	return (ST_OK);
}

/*
	apply_assings assign the vars to table depending on table type.
	- TMP means temp env table.
	- VAR means vars table. This situation, you don't touch the exported flag.
*/
t_status	apply_assigns(t_hashtable *table, t_assign *assign,
		t_tabletype type)
{
	t_bucket_contents	*item;

	if (type == TMP)
		hash_flush(table, NULL);
	while (assign)
	{
		item = hash_insert(assign->key->str, table);
		if (item == NULL)
			return (ST_FATAL);
		if (store_value(item, assign->value) != ST_OK)
			return (ST_FATAL);
		if (type == TMP)
			item->data.exported = true;
		assign = assign->next;
	}
	return (ST_OK);
}
