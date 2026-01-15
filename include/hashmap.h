/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 22:40:31 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/14 13:04:11 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdbool.h>
# define BUCKET_SIZE 128

// t_data has value and exported flag
// flag is used to distinguish "shell value" and "environ value"
typedef struct s_data
{
	char	*value;
	bool	exported;
}	t_data;

typedef struct s_bucket_contents	t_bucket_contents;

struct	s_bucket_contents
{
	t_bucket_contents	*next; // Link to next hashed key in this bucket.
	char				*key; // entry key (What we look up.)
	unsigned int		khash; // hashed key (What key hashes to)
	t_data				data; // entry data (What we really want.)
};

typedef struct s_hashtable			t_hashtable;

struct	s_hashtable
{
	t_bucket_contents	**bucket_array;//where the data is kept.
	int					bucket_size;// how many buckets does this table have.
	int					entry_count;// how many entries does this table have. 
};

/* hashmap_crud.c */
t_hashtable			*hash_create(int bucket_size);
t_bucket_contents	*hash_search(char *string, t_hashtable *table);
t_bucket_contents	*hash_remove(char *string, t_hashtable *table);
t_bucket_contents	*hash_insert(char *string, t_hashtable *table);
/* hashmap_free.c */
typedef void						t_free_func(void *);// Any free function
void				hash_flush(t_hashtable *table, t_free_func *free_data);
void				hash_dispose(t_hashtable *table);
/* hashmap_utils.c */
typedef int							t_hash_wfunc(t_bucket_contents *);
unsigned int		hash_string(const char *s); // Convert string to hash value
int					hash_bucket(unsigned int hash_value, t_hashtable *table);
								// hash value to index in table

#endif
