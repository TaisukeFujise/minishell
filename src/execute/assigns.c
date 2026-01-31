/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assigns.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 03:17:31 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/31 18:08:23 by tafujise         ###   ########.fr       */
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
t_status	apply_assings_to_vars(t_hashtable *env_table, t_word_list *assigns)
{
	char				*key;
	char				*value;
	t_bucket_contents	*item;

	while (assigns)
	{
		key = extract_key(assigns->wd);
		if (key == NULL)
			return (ST_FATAL);
		item = hash_insert(key, env_table);
		if (item == NULL)
		{
			free(key);
			key = NULL;
			return (ST_FATAL);
		}
		value = extract_value(assigns->wd);
		if (value == NULL)
			return (ST_FATAL);
		if (item->data.value != NULL)
		{
			free(item->data.value);
			item->data.value = NULL;
		}
		item->data.value =value;
		assigns = assigns->next;
	}
	return (ST_OK);
}

/*
	apply_assings_to_exec_env is called by exec_builtin_command and exec_disk_command.
	- Add temporary env variable to tmp table from assignment word.
*/
t_status	apply_assigns_to_exec_env(t_hashtable *tmp_table, t_word_list *assigns)
{
	char				*key;
	char				*value;
	t_bucket_contents	*item;

	hash_flush(tmp_table, NULL);
	while (assigns)
	{
		key = extract_key(assigns->wd);
		if (key == NULL)
			return (ST_FATAL);
		item = hash_insert(key, tmp_table);
		if (item == NULL)
		{
			free(key);
			key = NULL;
			return (ST_FATAL);
		}
		if (item->data.value != NULL)
		{
			free(item->data.value);
			item->data.value = NULL;
		}
		value = extrace_value(assigns->wd);
		if (value == NULL)
			return (ST_FATAL);
		item->data.value = value;
		item->data.exported = true;
		assigns = assigns->next;
	}
	return (ST_OK);
}

