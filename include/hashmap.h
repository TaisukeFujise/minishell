/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 22:40:31 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/15 03:38:08 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHMAP_H
# define HASHMAP_H

# include <stdbool.h>
# include <stdlib.h>
# define BUCKET_SIZE 128

// t_data has value and exported flag
// flag is used to distinguish "shell value" and "environ value"
typedef struct s_data
{
	char							*value;
	bool							exported;
}									t_data;

typedef struct s_bucket_contents	t_bucket_contents;

// next : Link to next hashed key in this bucket.
// key  : entry key (What we look up.)
// khash: hashed key (What key hashes to)
// data : entry data (What we really want.)
struct								s_bucket_contents
{
	t_bucket_contents				*next;
	char							*key;
	unsigned int					khash;
	t_data							data;
};

typedef struct s_hashtable			t_hashtable;

// bucket_array: where the data is kept.
// bucket_size : how many buckets does this table have.
// entry_count : how many entries does this table have.
struct								s_hashtable
{
	t_bucket_contents				**bucket_array;
	int								bucket_size;
	int								entry_count;
};

/* hashmap_crud.c */
t_hashtable							*hash_create(int bucket_size);
t_bucket_contents					*hash_search(char *string,
										t_hashtable *table);
t_bucket_contents					*hash_remove(char *string,
										t_hashtable *table);
t_bucket_contents					*hash_insert(char *string,
										t_hashtable *table);
bool								hash_set_value(t_bucket_contents *item,
										const char *value);
/* hashmap_free.c */
// t_free_func is any free function
typedef void						t_free_func(void *);
void								hash_flush(t_hashtable *table,
										t_free_func *free_data);
void								hash_dispose(t_hashtable *table);
/* hashmap_utils.c */
typedef int							t_hash_wfunc(t_bucket_contents *);
// hash_string converts a string to a hash value
unsigned int						hash_string(const char *s);
int									hash_bucket(unsigned int hash_value,
										t_hashtable *table);
void								hash_walk(t_hashtable *table,
										t_hash_wfunc *func);
t_bucket_contents					*hash_items(int bucket, t_hashtable *table);
// hash value to index in table

void								free_item(t_bucket_contents **item);

#endif
