/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lookup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 19:30:00 by fendo             #+#    #+#             */
/*   Updated: 2026/08/28 19:30:00 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "../../../include/hashmap.h"

/*
	Look a name up in the environment of the current command.
	The assignments of this command win over the persistent table.
	The returned value belongs to the table.
*/
char	*env_lookup(t_hashtable *tmp_table, t_hashtable *env_table, char *name)
{
	t_bucket_contents	*item;

	item = hash_search(name, tmp_table);
	if (item == NULL)
		item = hash_search(name, env_table);
	if (item == NULL)
		return (NULL);
	return (item->data.value);
}
