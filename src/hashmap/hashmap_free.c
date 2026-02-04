/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 02:55:49 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/14 13:57:16 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/hashmap.h"

/*
	free table content by using free_data func
	If free_data func is specified, use it.
	If NULL is specified by free_data func, free just item->data.value
*/
void	hash_flush(t_hashtable *table, t_free_func *free_data)
{
	int					i;
	t_bucket_contents	*bucket;
	t_bucket_contents	*item;

	if (table == NULL || table->bucket_array == NULL)
		return ;
	i = 0;
	while (i < table->bucket_size)
	{
		bucket = table->bucket_array[i];
		while (bucket != NULL)
		{
			item = bucket;
			bucket = bucket->next;
			if (free_data != NULL)
				(*free_data)(&item->data);
			else
				free(item->data.value);
			free(item->key);
			free(item);
		}
		table->bucket_array[i] = NULL;
		i++;
	}
	table->entry_count = 0;
}

/*
	free table itself.
	If table or table->bucket_array is NULL, do nothing.
*/
void	hash_dispose(t_hashtable *table)
{
	if (table == NULL || table->bucket_array == NULL)
		return ;
	free(table->bucket_array);
	free(table);
}
