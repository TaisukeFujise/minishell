/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_crud.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 21:45:48 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/14 13:35:58 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/hashmap.h"
#include "../../libft/libft.h"

// Make a new hash table with bucket_size.
// Initialize each slot in table to NULL
t_hashtable	*hash_create(int bucket_size)
{
	t_hashtable	*new_table;

	new_table = ft_calloc(1, sizeof(t_hashtable));
	if (new_table == NULL)
		return (NULL);
	new_table->bucket_array = ft_calloc(bucket_size,
			sizeof(t_bucket_contents *));
	if (new_table->bucket_array == NULL)
		return (free(new_table), NULL);
	new_table->bucket_size = bucket_size;
	return (new_table);
}

// Search existing key. If the key does'nt exist, return NULL.
t_bucket_contents	*hash_search(char *string, t_hashtable *table)
{
	unsigned int		hash_value;
	int					bucket;
	t_bucket_contents	*list;

	if (table == 0 || table->bucket_array == NULL)
		return (NULL);
	hash_value = hash_string(string);
	bucket = hash_bucket(hash_value, table);
	list = table->bucket_array[bucket];
	while (list != NULL)
	{
		if (hash_value == list->khash && ft_strcmp(string, list->key) == 0)
			return (list);
		list = list->next;
	}
	return (NULL);
}

// Remove existing key. (not free) You can free.
t_bucket_contents	*hash_remove(char *string, t_hashtable *table)
{
	unsigned int		hash_value;
	int					bucket;
	t_bucket_contents	*prev;
	t_bucket_contents	*tmp;

	if (table == 0 || table->bucket_array == NULL)
		return (NULL);
	hash_value = hash_string(string);
	bucket = hash_bucket(hash_value, table);
	prev = NULL;
	tmp = table->bucket_array[bucket];
	while (tmp != NULL)
	{
		if (hash_value == tmp->khash && ft_strcmp(string, tmp->key) == 0)
		{
			if (prev != NULL)
				prev->next = tmp->next;
			else
				table->bucket_array[bucket] = tmp->next;
			return (table->entry_count--, tmp);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (NULL);
}

/*
	Add new key or Overwrite existing key.
	It returns NULL if the table is not initialized, or
	memory allocation for item fails
*/
t_bucket_contents	*hash_insert(char *string, t_hashtable *table)
{
	t_bucket_contents	*item;
	unsigned int		hash_value;
	int					bucket;

	if (table == NULL || table->bucket_array == NULL)
		return (NULL);
	item = hash_search(string, table);
	if (item != NULL)
		return (item);
	hash_value = hash_string(string);
	bucket = hash_bucket(hash_value, table);
	item = ft_calloc(1, sizeof(t_bucket_contents));
	if (item == NULL)
		return (NULL);
	item->next = table->bucket_array[bucket];
	table->bucket_array[bucket] = item;
	item->key = string;
	item->khash = hash_value;
	table->entry_count++;
	return (item);
}
