/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 20:37:12 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/25 22:52:24 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execute.h"
#include "../../include/hashmap.h"

static int	_load_envp(t_hashtable *env_table, char **envp);

int	init_executor(t_exec *executor, char **envp)
{
	ft_bzero(executor, sizeof(t_exec));
	executor->env_table = hash_create(BUCKET_SIZE);
		return (FAILURE);
	if (_load_envp(executor->env_table, envp) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

static int	_load_envp(t_hashtable *env_table, char **envp)
{
	// TODO:w
	//
	// hash function makes envp key to hash value, and then hash value divided by hash table size is index of hash table.
	// hash table[index] has struct entry pointer.
	return (SUCCESS);
}
