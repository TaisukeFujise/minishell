/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assigns.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 03:17:31 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/31 18:50:37 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"
#include "../../include/execute.h"

/*
	apply_assigns_to_vars is called by exec_null_command.
	- Add shell variable to env_table from assignment word.
	- If there is already some entry, update the value.
	- This function doesn't touch export flag.
*/
t_status	apply_assings_to_vars(t_hashtable *env_table, t_assign *assign)
{
	t_bucket_contents	*item;

	while (assign)
	{
		item = hash_insert(assign->key, env_table);
		if (item == NULL)
			return (ST_FATAL);
		if (item->data.value != NULL)
		{
			free(item->data.value != NULL);
			item->data.value = NULL;
		}
		item->data.value =assign->value;
		assign = assign->next;
	}
	return (ST_OK);
}

/*
	apply_assings_to_exec_env is called by exec_builtin_command and exec_disk_command.
	- Add temporary env variable to tmp table from assignment word.
*/
t_status	apply_assigns_to_exec_env(t_hashtable *tmp_table, t_assign *assign)
{
	t_bucket_contents	*item;

	hash_flush(tmp_table, NULL);
	while (assign)
	{
		item = hash_insert(assign->key, tmp_table);
		if (item == NULL)
			return (ST_FATAL);
		if (item->data.value != NULL)
		{
			free(item->data.value);
			item->data.value = NULL;
		}
		item->data.value = assign->value;
		item->data.exported = true;
		assign = assign->next;
	}
	return (ST_OK);
}

