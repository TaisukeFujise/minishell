/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 20:37:12 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/27 01:51:26 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execute.h"
#include "../../include/hashmap.h"

static int	_load_envp(t_hashtable *env_table, char **envp);

int	init_executor(t_exec *executor, char **envp)
{
	ft_bzero(executor, sizeof(t_exec));
	executor->env_table = hash_create(BUCKET_SIZE);
	if (executor->env_table == NULL)
		return (FAILURE);
	if (_load_envp(executor->env_table, envp) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

typedef struct s_entry_view
{
	char	*key;
	int		key_len;
	char	*value;
	int		value_len;
}	t_entry_view;

void	extract_entry_view(char *entry, t_entry_view *entry_view)
{
	int				i;

	entry_view->key = entry;
	i = 0;
	while (entry[i] != '=' && entry[i] != '\0')
		i++;
	entry_view->key_len = i;
	if (entry[i] == '\0')
		return ;
	entry_view->value = &entry[i + 1];
	entry_view->value_len = ft_strlen(&entry[i + 1]);
}

static int	_load_envp(t_hashtable *env_table, char **envp)
{
	// TODO
	// hash table[index] has struct entry pointer.
	t_entry_view		entry_view;
	char				*key;
	char				*value;
	t_bucket_contents	*item;

	if (envp == NULL || *envp == NULL)
		return (FAILURE);
	while (*envp != NULL)
	{
		if (ft_strchr(*envp, '=') == NULL) // envp is reliable value, so we ignore the entry if the *envp doesn't have "="
			continue;
		extract_entry_view(*envp, &entry_view);
		key = ft_strndup(entry_view.key, entry_view.key_len);
		if (key == NULL)
			return (FAILURE); // free previous allocated memory!!!
		item = hash_insert(key, env_table);
		if (item == NULL)
			return (FAILURE); // free previous allocated memory !!!
		if (item->value != NULL)
			free(item->value);
		value = ft_strndup(entry_view.value, entry_view.value_len);
		if (value == NULL)
			return (FAILURE); // free previous allocated memory !!!
		item->value = value;
		item->exported = true;
		envp++;
	}
	return (SUCCESS);
}


