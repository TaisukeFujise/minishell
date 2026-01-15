/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 20:37:12 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/15 22:02:09 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execute.h"
#include "../../include/hashmap.h"

static int	_extract_entry_view(char *entry, t_entry_view *entry_view);
static int	_load_envp(t_hashtable *env_table, char **envp);

int	init_executor(t_exec *executor, char **envp)
{
	ft_bzero(executor, sizeof(t_exec));
	/* init fd used for pipe process */
	executor->input_fd = -1;
	executor->output_fd = -1;
	/* init env_table by envp */
	executor->env_table = hash_create(BUCKET_SIZE);
	if (executor->env_table == NULL)
		return (FAILURE);
	if (_load_envp(executor->env_table, envp) == FAILURE)
	{
		hash_flush(executor->env_table, NULL);
		hash_dispose(executor->env_table);
		return (FAILURE);
	}
	return (SUCCESS);
}

// envp is reliable value, so we ignore the entry if the *envp doesn't have "="
static int	_load_envp(t_hashtable *env_table, char **envp)
{
	t_entry_view		entry_view;
	char				*key;
	char				*value;
	t_bucket_contents	*item;

	if (envp == NULL)
		return (FAILURE);
	while (*envp != NULL)
	{
		if (_extract_entry_view(*envp, &entry_view) == FAILURE)
		{
			envp++;
			continue ;
		}
		key = ft_strndup(entry_view.key, entry_view.key_len);
		if (key == NULL)
			return (FAILURE);
		item = hash_insert(key, env_table);
		if (item == NULL)
		{
			free(key);
			key = NULL;
			return (FAILURE);
		}
		if (item->data.value != NULL)
		{
			free(item->data.value);
			item->data.value = NULL;
		}
		value = ft_strndup(entry_view.value, entry_view.value_len);
		if (value == NULL)
		{
			free(key);
			key = NULL;
			return (FAILURE);
		}
		item->data.value = value;
		item->data.exported = true;
		envp++;
	}
	return (SUCCESS);
}

static int	_extract_entry_view(char *entry, t_entry_view *entry_view)
{
	int	i;

	entry_view->key = entry;
	i = 0;
	while (entry[i] != '=' && entry[i] != '\0')
		i++;
	if (entry[i] == '\0')
		return (FAILURE);
	entry_view->key_len = i;
	entry_view->value = &entry[i + 1];
	entry_view->value_len = ft_strlen(&entry[i + 1]);
	return (SUCCESS);
}
