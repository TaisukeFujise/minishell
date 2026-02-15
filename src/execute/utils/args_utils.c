/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 02:38:46 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/16 01:14:19 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "../../../include/hashmap.h"
#include "../../../include/parser.h"

int	count_args(t_word_list *args)
{
	int	count;

	count = 0;
	if (args == NULL)
		return (count);
	while (args)
	{
		count++;
		args = args->next;
	}
	return (count);
}

char	**build_exec_argv(t_word_list *args)
{
	int		count;
	char	**av;
	int		i;

	if (args == NULL)
		return (NULL);
	count = count_args(args);
	av = ft_calloc(count + 1, sizeof(char *));
	if (av == NULL)
		return (NULL);
	i = 0;
	while (args)
	{
		av[i] = ft_strdup(args->wd->str);
		args = args->next;
		i++;
	}
	av[i] = NULL;
	return (av);
}

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
			if (item_tmp != NULL)
			{
				*envp++ = make_env_entry(item_tmp->key, item_tmp->data.value);
				if (*(envp - 1) == NULL)
					return (free_exec_params(NULL, head_envp), NULL);
			}
			else if (item->data.exported)
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

/*
	Build envp from tmp_table and env_table.
	- If there is an overlap between tmp_table and env_table,
		prioritize tmp_table.

*/
char	**build_exec_evnp(t_hashtable *tmp_table, t_hashtable *env_table)
{
	char	**envp;

	if (env_table == NULL && tmp_table == NULL)
		return (NULL);
	if (env_table == NULL)
	{
		envp = malloc(sizeof(char *) * (tmp_table->entry_count + 1));
		if (envp == NULL)
			return (NULL);
		return (table_to_envp(tmp_table, envp));
	}
	if (tmp_table == NULL)
	{
		envp = malloc(sizeof(char *) * (env_table->entry_count + 1));
		if (envp == NULL)
			return (NULL);
		return (table_to_envp(env_table, envp));
	}
	envp = malloc(sizeof(char *) * (tmp_table->entry_count
				+ env_table->entry_count + 1));
	if (envp == NULL)
		return (NULL);
	return (tables_to_envp(tmp_table, env_table, envp));
}

int	build_exec_params(t_exec_params *exec_params, t_word_list *args,
		t_hashtable *tmp_table, t_hashtable *env_table)
{
	exec_params->argv = build_exec_argv(args);
	if (exec_params->argv == NULL)
		return (FAILURE);
	exec_params->envp = build_exec_evnp(tmp_table, env_table);
	if (exec_params->envp == NULL)
		return (free_exec_params(exec_params->argv, exec_params->envp),
			FAILURE);
	return (SUCCESS);
}

void	free_exec_params(char **argv, char **envp)
{
	int	i;

	i = 0;
	if (argv != NULL)
	{
		while (argv[i])
		{
			free(argv[i]);
			argv[i] = NULL;
			i++;
		}
		free(argv);
	}
	if (envp != NULL)
	{
		i = 0;
		while (envp[i])
		{
			free(envp[i]);
			envp[i] = NULL;
			i++;
		}
		free(envp);
	}
}
