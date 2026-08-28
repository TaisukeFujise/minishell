/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 02:41:16 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/03 22:01:18 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/hashmap.h"
#include "../../libft/libft.h"

#define FNV_OFFSET 2166136261

/*
	FNV-1a hash function
*/
unsigned int	hash_string(const char *s)
{
	unsigned int	i;

	i = FNV_OFFSET;
	while (*s != '\0')
	{
		i += (i << 1) + (i << 4) + (i << 7) + (i << 8) + (i << 24);
		i ^= *s++;
	}
	return (i);
}

/*
	It returns hash value to index in the specified table.
*/
int	hash_bucket(unsigned int hash_value, t_hashtable *table)
{
	return (hash_value % table->bucket_size);
}

/*
	The entry whose key comes first after PREV, or NULL when none is
	left. A table keeps no order of its own, so finding one means looking
	at every entry; at this size that costs nothing and needs no array to
	sort.
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
			if ((prev == NULL || ft_strcmp(item->key, prev) > 0)
				&& (best == NULL || ft_strcmp(item->key, best->key) < 0))
				best = item;
			item = item->next;
		}
		i++;
	}
	return (best);
}

/*
	Walk the table in the order of its keys, applying func. Stops on the
	first func that answers below zero, and gives that answer back.
*/
int	hash_walk_ordered(t_hashtable *table, t_hash_wfunc *func)
{
	t_bucket_contents	*item;
	int					result;

	if (table == NULL || table->entry_count == 0)
		return (0);
	item = next_in_order(table, NULL);
	while (item != NULL)
	{
		result = (*func)(item);
		if (result < 0)
			return (result);
		item = next_in_order(table, item->key);
	}
	return (0);
}

/*
	It returns hash_item specified by bucket(index).
*/
t_bucket_contents	*hash_items(int bucket, t_hashtable *table)
{
	if (table != NULL && bucket < table->bucket_size)
		return (table->bucket_array[bucket]);
	return (NULL);
}
