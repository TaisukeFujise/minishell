/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_crud.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 21:45:48 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/26 03:05:07 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/hashmap.h"

// Make a new hash table with bucket_size.
// Initialize each slot in table to NULL
t_hashtable	*hash_create(int bucket_size)
{
	// TODO
	// create hashtable struct and return pointer by following buckets
	return (NULL);
}

// Add new key or Overwrite existing key.
t_bucket_contents	*hash_insert(char *string, t_hashtable *table)
{
	// TODO
	//
	return (NULL);
}

// Search existing key. If the key does'nt exist, return NULL.
t_bucket_contents	*hash_search(char *string, t_hashtable *table)
{
	// TODO
	return (NULL);
}

// Remove existing key. (not free) You can free.
t_bucket_contents	*hash_remove(char *string, t_hashtable *table)
{
	// TODO
	return (NULL);
}
