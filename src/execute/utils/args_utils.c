/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 02:38:46 by tafujise          #+#    #+#             */
/*   Updated: 2026/04/19 22:19:42 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "../../../include/hashmap.h"
#include "../../../include/parser.h"

char	**_build_exec_argv(t_word_list *args);
char	**_build_exec_evnp(t_hashtable *tmp_table, t_hashtable *env_table);

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

int	build_exec_params(t_exec_params *exec_params, t_word_list *args,
		t_hashtable *tmp_table, t_hashtable *env_table)
{
	exec_params->argv = _build_exec_argv(args);
	if (exec_params->argv == NULL)
		return (FAILURE);
	exec_params->envp = _build_exec_evnp(tmp_table, env_table);
	if (exec_params->envp == NULL)
		return (free_exec_params(exec_params->argv, exec_params->envp),
			FAILURE);
	return (SUCCESS);
}

static char	*word_to_str(t_word *wd)
{
	t_word	*part;
	int		total;
	char	*str;
	int		i;

	total = 0;
	part = wd;
	while (part)
	{
		total += part->len;
		part = part->next;
	}
	str = malloc(total + 1);
	if (str == NULL)
		return (NULL);
	i = 0;
	part = wd;
	while (part)
	{
		ft_memcpy(str + i, part->str, part->len);
		i += part->len;
		part = part->next;
	}
	str[total] = '\0';
	return (str);
}

char	**_build_exec_argv(t_word_list *args)
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
		av[i] = word_to_str(args->wd);
		if (av[i] == NULL)
			return (free_exec_params(av, NULL), NULL);
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
char	**_build_exec_evnp(t_hashtable *tmp_table, t_hashtable *env_table)
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
