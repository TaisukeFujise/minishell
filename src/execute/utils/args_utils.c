/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 02:38:46 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/15 16:02:36 by tafujise         ###   ########.fr       */
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

/*
	Build envp from tmp_table and env_table.
	- If there is an overlap between tmp_table and env_table,
		prioritize tmp_table.
*/
char	**build_exec_evnp(t_hashtable *tmp_table, t_hashtable *env_table,
		int total_entry)
{
	int					i;
	char				**envp;
	char				**head_envp;
	t_bucket_contents	*item;
	t_bucket_contents	*item_tmp;

	if (env_table == 0 || env_table->entry_count == 0)
		return (NULL);
	envp = ft_calloc(total_entry, sizeof(char *));
	if (envp == NULL)
		return (NULL);
	head_envp = envp;
	i = 0;
	while (i < env_table->bucket_size)
	{
		item = hash_items(i, env_table);
		while (item)
		{
			item_tmp = hash_search(item->key, tmp_table);
			if (item_tmp == NULL)
				*envp = ft_strdup(item->data.value);
			else
				*envp = ft_strdup(item_tmp->data.value);
			free_item(&item_tmp);
			if (*envp == NULL)
				return (free_exec_params(NULL, head_envp), NULL);
			envp++;
			item = item->next;
		}
		i++;
	}
	i = 0;
	while (i < tmp_table->bucket_size)
	{
		item = hash_items(i, env_table);
		while (item)
		{
			*envp = ft_strdup(item->data.value);
			if (*envp == NULL)
				return (free_exec_params(NULL, head_envp), NULL);
			envp++;
			item = item->next;
		}
		i++;
	}
	return (head_envp);
}

int	build_exec_params(t_exec_params *exec_params, t_word_list *args,
		t_hashtable *tmp_table, t_hashtable *env_table)
{
	int	total_entry;

	puts("b1");
	exec_params->argv = build_exec_argv(args);
	if (exec_params->argv == NULL)
		return (FAILURE);
	puts("b2");
	total_entry = 0;
	if (tmp_table != NULL)
		total_entry += tmp_table->entry_count;
	if (env_table != NULL)
		total_entry += env_table->entry_count;
	puts("b3");
	exec_params->envp = build_exec_evnp(tmp_table, env_table, total_entry);
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
		while (envp[i])
		{
			free(envp[i]);
			envp[i] = NULL;
			i++;
		}
		free(envp);
	}
}
