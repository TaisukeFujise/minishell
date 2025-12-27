/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 02:41:16 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/27 18:21:15 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/hashmap.h"

#define FNV_OFFSET 2166136261

// FNV-1a hash function
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

// hash value to index in table
int	hash_bucket(unsigned int hash_value, t_hashtable *table)
{
	return (hash_value % table->bucket_size);
}

void	hash_walk(t_hashtable *table, t_hash_wfunc *func)
{
	int					i;
	t_bucket_contents	*item;

	if (table == 0 || table->entry_count == 0)
		return ;
	i = 0;
	while (i < table->bucket_size)
	{
		while (item != NULL)
		{
			if ((*func)(item) < 0)
				return ;
			item = item->next;
		}
		i++;
	}
}
