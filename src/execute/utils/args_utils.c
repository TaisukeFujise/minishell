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
#include "../../../include/variables.h"
#include "../../../include/hashmap.h"
#include "../../../include/parser.h"

static int	count_args(t_word_list *args)
{
	int	count;

	count = 0;
	while (args)
	{
		count++;
		args = args->next;
	}
	return (count);
}

void	free_exec_params(char **argv, char **envp)
{
	free(argv);
	free_envp(envp);
}

/*
	argv points at the expanded words, which the ast arena owns,
	so only the pointer array itself is allocated here.
*/
static char	**build_argv(t_word_list *args)
{
	char	**argv;
	int		i;

	if (args == NULL)
		return (NULL);
	argv = ft_calloc(count_args(args) + 1, sizeof(char *));
	if (argv == NULL)
		return (NULL);
	i = 0;
	while (args)
	{
		argv[i] = args->wd->str;
		args = args->next;
		i++;
	}
	return (argv);
}

int	build_exec_params(t_exec_params *exec_params, t_word_list *args,
		t_hashtable *tmp_table, t_hashtable *env_table)
{
	exec_params->argv = build_argv(args);
	if (exec_params->argv == NULL)
		return (FAILURE);
	exec_params->envp = build_envp(tmp_table, env_table);
	if (exec_params->envp == NULL)
		return (free(exec_params->argv), FAILURE);
	return (SUCCESS);
}
