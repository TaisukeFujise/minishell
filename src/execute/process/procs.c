/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   register_pid.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 00:09:40 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/11 11:04:17 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

bool	procs_init(t_procs *procs, int capacity)
{
	procs->pids = ft_calloc(capacity, sizeof(pid_t));
	if (procs->pids == NULL)
		return (false);
	procs->count = 0;
	procs->capacity = capacity;
	return (true);
}

void	procs_free(t_procs *procs)
{
	free(procs->pids);
	procs->pids = NULL;
	procs->count = 0;
	procs->capacity = 0;
}

/*
	The room was taken before the first fork, so a child always has a
	place here. [review D37-18, PROC-01]
*/
t_status	procs_add(t_procs *procs, pid_t pid)
{
	if (procs->count >= procs->capacity)
		return (ST_FATAL);
	procs->pids[procs->count] = pid;
	procs->count++;
	return (ST_OK);
}
