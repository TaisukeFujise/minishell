/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 02:41:16 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/27 15:43:55 by tafujise         ###   ########.fr       */
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
