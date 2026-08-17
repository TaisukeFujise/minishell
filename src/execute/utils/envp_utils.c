/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:39:35 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/16 02:52:57 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "../../../include/hashmap.h"
#include "../../../include/parser.h"

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

/*
	Walk one table and append the entries the merged environment keeps.
	From env_table we skip the keys tmp_table overrides, they come from
	the tmp_table walk. Entries that are not exported are shell variables.
*/
static char	**walk_table(t_hashtable *table, t_hashtable *skip, char **envp)
{
	int					i;
	t_bucket_contents	*item;

	i = 0;
	while (table != NULL && i < table->bucket_size)
	{
		item = hash_items(i, table);
		while (item != NULL)
		{
			if (item->data.exported
				&& (skip == NULL || hash_search(item->key, skip) == NULL))
			{
				*envp = make_env_entry(item->key, item->data.value);
				if (*envp == NULL)
					return (NULL);
				envp++;
			}
			item = item->next;
		}
		i++;
	}
	return (envp);
}

/*
	Build the environment of the current command from the persistent table
	and the assignments of this command. tmp_table wins on the same key.
	The result is a NULL terminated array the caller owns.
*/
char	**build_envp(t_hashtable *tmp_table, t_hashtable *env_table)
{
	char	**envp;
	char	**tail;
	int		count;

	count = 1;
	if (tmp_table != NULL)
		count += tmp_table->entry_count;
	if (env_table != NULL)
		count += env_table->entry_count;
	envp = ft_calloc(count, sizeof(char *));
	if (envp == NULL)
		return (NULL);
	tail = walk_table(tmp_table, NULL, envp);
	if (tail != NULL)
		tail = walk_table(env_table, tmp_table, tail);
	if (tail == NULL)
		return (free_envp(envp), NULL);
	*tail = NULL;
	return (envp);
}

void	free_envp(char **envp)
{
	int	i;

	i = 0;
	if (envp == NULL)
		return ;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}
