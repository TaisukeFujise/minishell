/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 02:55:49 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/27 16:10:27 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/hashmap.h"

// free table content by using free_data func
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
			free(item->key);
			free(item);
		}
		table->bucket_array[i] = NULL;
		i++;
	}
	table->entry_count = 0;
}

// free table itself
void	hash_dispose(t_hashtable *table)
{
	if (table == NULL || table->bucket_array == NULL)
		return ;
	free(table->bucket_array);
	free(table);
}
