/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:39:30 by tafujise          #+#    #+#             */
/*   Updated: 2026/05/10 18:37:03 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "hashmap.h"

void		print_env_from_envtable(t_hashtable *tmp_table,
				t_hashtable *env_table);
void		print_env_from_tmptable(t_hashtable *tmp_table,
				t_hashtable *env_table);
void		print_entry(t_bucket_contents *item);

/*
	env
	"env" display the env table, and update the ctx->exit_code.
*/
t_status	env_cmd(t_word_list *args, t_ctx *ctx)
{
	if (args != NULL)
		return (ST_FAILURE); // env: too many arguments
	if (args == NULL)
	{
		print_env_from_envtable(ctx->tmp_table, ctx->env_table);
		print_env_from_tmptable(ctx->tmp_table, ctx->env_table);
	}
	return (ST_OK);
}

void	print_env_from_envtable(t_hashtable *tmp_table, t_hashtable *env_table)
{
	int					i;
	t_bucket_contents	*item;
	t_bucket_contents	*item_tmp;

	if (env_table == 0 || env_table->entry_count == 0)
		return ;
	i = 0;
	while (i < env_table->bucket_size)
	{
		item = hash_items(i, env_table);
		while (item != NULL)
		{
			item_tmp = hash_search(item->key, tmp_table);
			if (item_tmp == NULL)
				print_entry(item);
			else
			{
				print_entry(item_tmp);
			}
			item = item->next;
		}
		i++;
	}
}

void	print_env_from_tmptable(t_hashtable *tmp_table, t_hashtable *env_table)
{
	int					i;
	t_bucket_contents	*item;

	i = 0;
	if (tmp_table == NULL)
		return ;
	while (i < tmp_table->bucket_size)
	{
		item = hash_items(i, tmp_table);
		while (item != NULL)
		{
			if (hash_search(item->key, env_table) == NULL)
			{
				print_entry(item);
			}
			item = item->next;
		}
		i++;
	}
}

void	print_entry(t_bucket_contents *item)
{
	if (item->data.exported)
	{
		if (item->data.value != NULL)
		{
			printf("%s=%s\n", item->key, item->data.value);
		}
	}
}
