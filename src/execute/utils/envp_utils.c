/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 02:18:04 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/16 02:56:38 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "../../../include/hashmap.h"
#include "../../../include/parser.h"

static int	_item_to_envp(t_bucket_contents *item, t_bucket_contents *item_tmp,
				char ***envp);

char	*make_env_entry(char *key, char *value)
{
	char	*entry;
	char	*head;
	int		i;

	if (key == NULL || value == NULL)
		return (NULL);
	entry = malloc(sizeof(char) * (ft_strlen(key) + ft_strlen(value) + 2));
	if (entry == NULL)
		return (NULL);
	i = 0;
	head = entry;
	while (key[i])
	{
		*entry++ = key[i];
		i++;
	}
	*entry++ = '=';
	i = 0;
	while (value[i])
	{
		*entry++ = value[i];
		i++;
	}
	*entry = '\0';
	return (head);
}

char	**table_to_envp(t_hashtable *table, char **envp)
{
	char				**head_envp;
	t_bucket_contents	*item;
	int					i;

	head_envp = envp;
	i = 0;
	while (i++ < table->bucket_size)
	{
		item = hash_items(i - 1, table);
		while (item)
		{
			if (item->data.exported)
			{
				*envp++ = make_env_entry(item->key, item->data.value);
				if (*(envp - 1) == NULL)
					return (free_exec_params(NULL, head_envp), NULL);
			}
			item = item->next;
		}
	}
	*envp = NULL;
	return (head_envp);
}

char	**tables_to_envp(t_hashtable *tmp_table, t_hashtable *env_table,
		char **envp)
{
	char				**head_envp;
	t_bucket_contents	*item;
	t_bucket_contents	*item_tmp;
	int					i;

	head_envp = envp;
	i = 0;
	while (i++ < env_table->bucket_size)
	{
		item = hash_items(i - 1, env_table);
		while (item)
		{
			item_tmp = hash_search(item->key, tmp_table);
			if (_item_to_envp(item, item_tmp, &envp) == FAILURE)
				return (free_exec_params(NULL, head_envp), NULL);
			item = item->next;
		}
	}
	*envp = NULL;
	return (head_envp);
}

static int	_item_to_envp(t_bucket_contents *item, t_bucket_contents *item_tmp,
		char ***envp)
{
	if (item_tmp != NULL)
	{
		**envp = make_env_entry(item_tmp->key, item_tmp->data.value);
		if (**envp == NULL)
			return (FAILURE);
		(*envp)++;
	}
	else if (item->data.exported)
	{
		**envp = make_env_entry(item->key, item->data.value);
		if (**envp == NULL)
			return (FAILURE);
		(*envp)++;
	}
	return (SUCCESS);
}
