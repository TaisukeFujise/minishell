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
		if (item->data.value != NULL)
		{
			free(item->data.value);
			item->data.value = NULL;
		}
		item->data.value = assign->value->str;
		if (type == TMP)
			item->data.exported = true;
		assign = assign->next;
	}
	return (ST_OK);
}
